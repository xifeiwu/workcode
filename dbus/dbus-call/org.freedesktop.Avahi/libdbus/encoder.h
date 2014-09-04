#ifndef ENCODER_H_
#define ENCODER_H_


const char *GetSignatureFromV8Type(Local<Value>& value);
bool EncodeObject(Handle<Value> value, DBusMessageIter *iter, const char *signature);

#endif
