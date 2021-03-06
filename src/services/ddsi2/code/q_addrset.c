#include <string.h>
#include <stddef.h>

#include "os_heap.h"
#include "os_socket.h"
#include "os_stdlib.h"

#include "q_avl.h"
#include "q_log.h"
#include "q_misc.h"
#include "q_config.h"
#include "q_addrset.h"
#include "q_globals.h" /* gv.mattr */

/* So what does one do with const & mutexes? I need to take lock in a
   pure function just in case some other thread is trying to change
   something. Arguably, that means the thing isn't const; but one
   could just as easily argue that "const" means "this call won't
   change it". If it is globally visible before the call, it may
   change anyway.

   Today, I'm taking the latter interpretation. But all the
   const-discarding casts get moved into LOCK/UNLOCK macros. */
#define LOCK(as) (os_mutexLock (&((struct addrset *) (as))->lock))
#define TRYLOCK(as) (os_mutexTryLock (&((struct addrset *) (as))->lock))
#define UNLOCK(as) (os_mutexUnlock (&((struct addrset *) (as))->lock))

void init_locator (nn_locator_t *loc, const os_sockaddr_storage *addr, unsigned short port)
{
  /* addr in proper network format; we keep the address part of the
     locators in proper network format, but not the kind and port fields */
  if (addr->ss_family == AF_INET)
  {
    memset (loc, 0, sizeof (*loc));
    loc->kind = NN_LOCATOR_KIND_UDPv4;
    memcpy (loc->address + 12, &((os_sockaddr_in *) addr)->sin_addr, 4);
    loc->port = port;
  }
  else
  {
    memset (loc, 0, sizeof (*loc));
    loc->kind = NN_LOCATOR_KIND_UDPv6;
    memcpy (loc->address, &((os_sockaddr_in6 *) addr)->sin6_addr, 16);
    loc->port = port;
  }
}

static int add_addresses_to_addrset_1 (struct addrset *as, const char *ip, int port_mode, const char *msgtag)
{
  char buf[INET6_ADDRSTRLEN_EXTENDED];
  os_sockaddr_storage addr;

  if (!os_sockaddrStringToAddress (ip, (os_sockaddr *) &addr, !config.useIpv6))
  {
    NN_WARNING2 ("%s: %s: not a valid address\n", msgtag, ip);
    return -1;
  }

  if (port_mode >= 0)
  {
    sockaddr_set_port (&addr, port_mode);
    nn_log (LC_CONFIG, "%s: add %s", msgtag, sockaddr_to_string_with_port (buf, &addr));
    add_to_addrset (as, &addr);
  }
  else
  {
    sockaddr_set_port (&addr, 0);
    nn_log (LC_CONFIG, "%s: add ", msgtag);
    if (!is_mcaddr (&addr))
    {
      int i;
      for (i = 0; i < 10; i++)
      {
        int port = config.port_base + config.port_dg * config.domainId + i * config.port_pg + config.port_d1;
        sockaddr_set_port (&addr, port);
        if (i == 0)
          nn_log (LC_CONFIG, "%s", sockaddr_to_string_with_port (buf, &addr));
        else
          nn_log (LC_CONFIG, ", :%d", port);
        add_to_addrset (as, &addr);
      }
    }
    else
    {
      int port = port_mode;
      if (port == -1)
        port = config.port_base + config.port_dg * config.domainId + config.port_d0;
      sockaddr_set_port (&addr, port);
      nn_log (LC_CONFIG, "%s", sockaddr_to_string_with_port (buf, &addr));
      add_to_addrset (as, &addr);
    }
  }

  nn_log (LC_CONFIG, "\n");
  return 0;
}

int add_addresses_to_addrset (struct addrset *as, const char *addrs, int port_mode, const char *msgtag)
{
  /* port_mode: -1  => take from string, if 0 & unicast, add for a range of participant indices;
     port_mode >= 0 => always set port to port_mode
  */
  char *addrs_copy, *ip, *cursor, *a;
  int retval = -1;
  if ((addrs_copy = os_strdup (addrs)) == NULL)
    return -1;
  if ((ip = os_malloc (strlen (addrs) + 1)) == NULL)
  {
    os_free (addrs_copy);
    return -1;
  }
  cursor = addrs_copy;
  while ((a = ddsi2_strsep (&cursor, ",")) != NULL)
  {
    int port = 0, pos;
    if (!config.useIpv6)
    {
      if (port_mode == -1 && sscanf (a, "%[^:]:%d%n", ip, &port, &pos) == 2 && a[pos] == 0)
        ; /* XYZ:PORT */
      else if (sscanf (a, "%[^:]%n", ip, &pos) == 1 && a[pos] == 0)
        port = port_mode; /* XYZ */
      else { /* XY:Z -- illegal, but conversion routine should flag it */
        strcpy (ip, a);
        port = 0;
      }
    }
    else
    {
      if (port_mode == -1 && sscanf (a, "[%[^]]]:%d%n", ip, &port, &pos) == 2 && a[pos] == 0)
        ; /* [XYZ]:PORT */
      else if (sscanf (a, "[%[^]]]%n", ip, &pos) == 1 && a[pos] == 0)
        port = port_mode; /* [XYZ] */
      else { /* XYZ -- let conversion routines handle errors */
        strcpy (ip, a);
        port = 0;
      }
    }

    if ((port > 0 && port <= 65535) || (port_mode == -1 && port == -1)) {
      if (add_addresses_to_addrset_1 (as, ip, port, msgtag) < 0)
        goto error;
    } else {
      NN_WARNING3 ("%s: %s: port %d invalid\n", msgtag, a, port);
    }
  }
  retval = 0;
 error:
  os_free (ip);
  os_free (addrs_copy);
  return retval;
}

static int compare_addr (const void *va, const void *vb)
{
  const os_sockaddr_storage *a = va;
  const os_sockaddr_storage *b = vb;

  if (a->ss_family != b->ss_family)
    return (int) a->ss_family - (int) b->ss_family;

  switch (a->ss_family)
  {
    case AF_INET:
      {
        const os_sockaddr_in *a4 = va;
        const os_sockaddr_in *b4 = vb;
        int x = (int) a4->sin_addr.s_addr - (int) b4->sin_addr.s_addr;
        if (x != 0)
          return x;
        else
          return (int) a4->sin_port - (int) b4->sin_port;
      }
#if OS_SOCKET_HAS_IPV6
    case AF_INET6:
      {
        const os_sockaddr_in6 *a6 = va;
        const os_sockaddr_in6 *b6 = vb;
        int x = memcmp (&a6->sin6_addr.s6_addr, &b6->sin6_addr.s6_addr, 16);
        if (x != 0)
          return x;
        else
          return (int) a6->sin6_port - (int) b6->sin6_port;
      }
#endif
    default:
      assert (0);
      return 0;
  }
}

struct addrset *new_addrset (void)
{
  struct addrset *as = os_malloc (sizeof (*as));
  as->refc = 1;
  os_mutexInit (&as->lock, &gv.mattr);
  avl_init (&as->ucaddrs, offsetof (struct addrset_node, avlnode), offsetof (struct addrset_node, addr), compare_addr, 0);
  avl_init (&as->mcaddrs, offsetof (struct addrset_node, avlnode), offsetof (struct addrset_node, addr), compare_addr, 0);
  return as;
}

struct addrset *ref_addrset (struct addrset *as)
{
  if (as != NULL)
    pa_increment (&as->refc);
  return as;
}

void unref_addrset (struct addrset *as)
{
  if (pa_decrement (&as->refc) == 0)
  {
    avl_free (&as->ucaddrs, os_free);
    avl_free (&as->mcaddrs, os_free);
    os_mutexDestroy (&as->lock);
    os_free (as);
  }
}

int is_mcaddr (const os_sockaddr_storage *addr)
{
  switch (addr->ss_family)
  {
    case AF_INET:
      {
        const os_sockaddr_in *x = (const os_sockaddr_in *) addr;
        return IN_MULTICAST (ntohl (x->sin_addr.s_addr));
      }
#if OS_SOCKET_HAS_IPV6
    case AF_INET6:
      {
        const os_sockaddr_in6 *x = (const os_sockaddr_in6 *) addr;
        return IN6_IS_ADDR_MULTICAST (&x->sin6_addr);
      }
#endif
    default:
      assert (0);
      return 0;
  }
}

int addrset_purge (struct addrset *as)
{
  LOCK (as);
  avl_free (&as->ucaddrs, os_free);
  avl_free (&as->mcaddrs, os_free);
  UNLOCK (as);
  return 0;
}

void add_to_addrset (struct addrset *as, const os_sockaddr_storage *addr)
{
  avlparent_t parent;
  struct addrset_avltree *tree = is_mcaddr (addr) ? &as->mcaddrs : &as->ucaddrs;
  LOCK (as);
  if (avl_lookup (tree, addr, &parent) == NULL)
  {
    struct addrset_node *n = os_malloc (sizeof (*n));
    avl_init_node (&n->avlnode, parent);
    n->addr = *addr;
    avl_insert (tree, n);
  }
  UNLOCK (as);
}

static void copy_addrset_into_addrset_helper (void *vnode, void *varg)
{
  const struct addrset_node *n = vnode;
  add_to_addrset (varg, &n->addr);
}

void copy_addrset_into_addrset_uc (struct addrset *as, const struct addrset *asadd)
{
  LOCK (asadd);
  avl_walk ((struct addrset_avltree *) &asadd->ucaddrs, copy_addrset_into_addrset_helper, as);
  UNLOCK (asadd);
}

void copy_addrset_into_addrset_mc (struct addrset *as, const struct addrset *asadd)
{
  LOCK (asadd);
  avl_walk ((struct addrset_avltree *) &asadd->mcaddrs, copy_addrset_into_addrset_helper, as);
  UNLOCK (asadd);
}

void copy_addrset_into_addrset (struct addrset *as, const struct addrset *asadd)
{
  copy_addrset_into_addrset_uc (as, asadd);
  copy_addrset_into_addrset_mc (as, asadd);
}

int addrset_empty_uc (const struct addrset *as)
{
  int isempty;
  LOCK (as);
  isempty = avl_empty (&as->ucaddrs);
  UNLOCK (as);
  return isempty;
}

int addrset_empty_mc (const struct addrset *as)
{
  int isempty;
  LOCK (as);
  isempty = avl_empty (&as->mcaddrs);
  UNLOCK (as);
  return isempty;
}

int addrset_empty (const struct addrset *as)
{
  int isempty;
  LOCK (as);
  isempty = avl_empty (&as->ucaddrs) && avl_empty (&as->mcaddrs);
  UNLOCK (as);
  return isempty;
}

int addrset_any_uc (const struct addrset *as, os_sockaddr_storage *dst)
{
  LOCK (as);
  if (avl_empty (&as->ucaddrs))
  {
    UNLOCK (as);
    return 0;
  }
  else
  {
    *dst = as->ucaddrs.root->addr;
    UNLOCK (as);
    return 1;
  }
}

int addrset_any_mc (const struct addrset *as, os_sockaddr_storage *dst)
{
  LOCK (as);
  if (avl_empty (&as->mcaddrs))
  {
    UNLOCK (as);
    return 0;
  }
  else
  {
    *dst = as->mcaddrs.root->addr;
    UNLOCK (as);
    return 1;
  }
}

struct addrset_forall_addresses_helper_arg {
  addrset_forall_fun_t f;
  void *arg;
  size_t ret;
};

static void addrset_forall_helper (void *vnode, void *varg)
{
  const struct addrset_node *n = vnode;
  struct addrset_forall_addresses_helper_arg *arg = varg;
  arg->ret += arg->f (&n->addr, arg->arg);
}

size_t addrset_forall_addresses (struct addrset *as, addrset_forall_fun_t f, void *arg)
{
  struct addrset_forall_addresses_helper_arg arg1;
  arg1.f = f;
  arg1.arg = arg;
  arg1.ret = 0;
  LOCK (as);
  avl_walk (&as->mcaddrs, addrset_forall_helper, &arg1);
  avl_walk (&as->ucaddrs, addrset_forall_helper, &arg1);
  UNLOCK (as);
  return arg1.ret;
}

struct log_addrset_helper_arg {
  logcat_t tf;
};

static size_t log_addrset_helper (const os_sockaddr_storage *n, void *varg)
{
  const struct log_addrset_helper_arg *arg = varg;
  char buf[INET6_ADDRSTRLEN_EXTENDED];
  if (config.enabled_logcats & arg->tf)
    nn_log (arg->tf, " %s", sockaddr_to_string_with_port (buf, n));
  return 0;
}

void nn_log_addrset (logcat_t tf, const char *prefix, const struct addrset *as)
{
  struct log_addrset_helper_arg arg;
  arg.tf = tf;
  nn_log (tf, prefix);
  addrset_forall_addresses ((struct addrset *) as, log_addrset_helper, &arg); /* drop const, we know it is */
}

static int addrset_eq_onesidederr1 (const struct addrset_avltree *at, const struct addrset_avltree *bt)
{
  /* Just checking the root */
  if (at->root == NULL)
    return (bt->root == NULL);
  else if (bt->root == NULL)
    return 0;
  else
    return
      (at->root->avlnode.left == NULL && at->root->avlnode.right == NULL &&
       bt->root->avlnode.left == NULL && bt->root->avlnode.right == NULL &&
       compare_addr (&at->root->addr, &bt->root->addr) == 0);
}

int addrset_eq_onesidederr (const struct addrset *a, const struct addrset *b)
{
  int iseq;
  if (a == b)
    return 1;
  LOCK (a);
  if (TRYLOCK (b) == os_resultSuccess)
  {
    iseq =
      addrset_eq_onesidederr1 (&a->ucaddrs, &b->ucaddrs) &&
      addrset_eq_onesidederr1 (&a->mcaddrs, &b->mcaddrs);
    UNLOCK (b);
  }
  else
  {
    /* We could try <lock b ; trylock(a)>, in a loop, &c. Or we can
       just decide it isn't worth the bother. Which it isn't because
       it doesn't have to be an exact check on equality. A possible
       improvement would be to use an rwlock. */
    iseq = 0;
  }
  UNLOCK (a);
  return iseq;
}

/* SHA1 not available (unoffical build.) */
