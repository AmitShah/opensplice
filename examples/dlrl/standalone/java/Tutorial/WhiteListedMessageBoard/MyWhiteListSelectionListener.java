package WhiteListedMessageBoard;

/**
 * This class implements the DLRL specific SelectionListener interface, this
 * listener is used within the WhiteListedMessageBoard application to get
 * fine grained notification every time a WhiteList object that matches the
 * filter belonging to the selection this listener is attached too, enters or
 * exits the selection.
 * This listener can be set to a WhiteListSelection and will then be
 * triggered each time something is changed within the selection. Assuming the
 * cache has updates enabled (meaning that a call to the cache 'updates_enabled'
 * operation returns true).
 */
public class MyWhiteListSelectionListener implements DLRLChat.WhiteListSelectionListener
{
    /* Contains the message board object relevant to this specific listener
     * so it can invoke operations on the messageboard to set or unset
     * the whitelist.
     */
    private WhiteListedMessageBoard msgBoard;

    /* Constructor for the listener
     *
     * @param msgBoard the WhiteListedMessageBoard this listener is linked too.
     */
    public MyWhiteListSelectionListener(
        WhiteListedMessageBoard msgBoard)
    {
        this.msgBoard = msgBoard;
    }

    /**
     * Will be invoked by the associated {@link DLRLChat.WhiteListSelection}
     * when an object of type DLRLChat.WhiteList, or one of its sub-classes
     * becomes a member of this selection (i.e. an object is detected to meet
     * the criteria of the selection for the first time).
     *
     * @param the_object the reference to the new Object member.
     */
    public void on_object_in (
        DLRLChat.WhiteList the_object)
    {
        /* It is very important to catch ALL exceptions within the scope of the
         * listener operation. If this is not done the exception will be
         * propagated into the DLRL kernel, where the DLRL will report the
         * error to an error log and shutdown the thread handling updates
         * due to the exception being a fatal error, as any exception thrown
         * by this operation is by definition an application induced exception
         * for which there is no generic way of recovery.
         */
        try
        {
            /* When we find a white list that matches our filter, we will
             * notify this to the messageboard.
             */
            msgBoard.setWhiteList(the_object);
        }
        catch (Exception e)
        {
            /* Any sort of exception handling may occur here, this is
             * application driven. In our example the exception will be
             * simply caught and ignored.
             */
        }
    }

    /**
     * Will be invoked by the associated {@link DLRLChat.WhiteListSelection}
     * when an object of type DLRLChat.WhiteList, or one of its sub-classes
     * which already was a member of the associated Selection has been modified
     * in the update round (i.e. an object has been modified but still meets
     * the criteria of the selection).
     *
     * @param the_object the reference to the modified Object.
     */
    public void on_object_modified (
        DLRLChat.WhiteList the_object)
    {
        try
        {
            /* do nothing, we're not interested in this event */
        }
        catch (Exception e)
        {
            /* Any sort of exception handling may occur here, this is
             * application driven. In our example the exception will be
             * simply caught and ignored.
             */
        }
    }

    /**
     * Will be invoked by the associated {@link DLRLChat.WhiteListSelection}
     * when an object of type DLRLChat.WhiteList, or one of its sub-classes
     * leaves this selection (i.e. no longer meets the criteria).
     *
     * @param the_object the reference to the deleted Object.
     */
    public void on_object_out (
        DLRLChat.WhiteList the_object)
    {
        try
        {
            msgBoard.setWhiteList(null);
        }
        catch (Exception e)
        {
            /* Any sort of exception handling may occur here, this is
             * application driven. In our example the exception will be
             * simply caught and ignored.
             */
        }
    }

}