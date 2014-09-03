//Parsing a signature s(i{ii}i)u
DBusMessageIter rootIter;
dbus_message_iter_init(msg, &rootIter);

if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&rootIter))
{
    char* str = NULL; 
    dbus_message_iter_get_basic(&rootIter, &str);//this function is used to read basic dbus types like int, string etc. 
    dbus_message_iter_next(&rootIter);//Go to next argument of root iter

    //Block to enter and read structure
    if (DBUS_TYPE_STRUCT == dbus_message_iter_get_arg_type(&rootIter))
    {
    DBusMessageIter structIter;
    dbus_message_iter_recurse(&rootIter, &structIter);//Initialize iterator for struct

    //Argument 1 is int32
    if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&structIter))
    {
        int a;
            dbus_message_iter_get_basic(&structIter, &a);//Read integer
        dbus_message_iter_next(&structIter);//Go to next argument of structiter

        if (DDBUS_TYPE_DICT_ENTRY == dbus_message_iter_get_arg_type(&structIter))
        {
           DBusMessageIter dictIter;           
               dbus_message_iter_recurse(&structIter, &dictIter);//Initialize iterator for dictentry
           if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&dictIter))
           {
            dbus_message_iter_get_basic(&dictIter, &a);//Read integer
            dbus_message_iter_next(&dictIter);//Go to next argument of dictentry
            if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&dictIter))
            {
               dbus_message_iter_get_basic(&dictIter, &a);//Read integer
            }
           }
        }
        dbus_message_iter_next(&structIter);//Go to next argument of structiter
            if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&structIter))
        {
           dbus_message_iter_get_basic(&structIter, &a);//Read integer
        }
        }
    }
    dbus_message_iter_next(&rootIter);//Go to next argument of root iterator
    if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&rootIter))
    {
    uint32_t b;
    dbus_message_iter_get_basic(&rootIter, &b);//Read integer
    }
}
