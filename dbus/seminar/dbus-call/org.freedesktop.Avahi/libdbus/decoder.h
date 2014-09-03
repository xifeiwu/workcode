#ifndef DECODER_H_
#define DECODER_H_

Handle<Value> DecodeMessage(DBusMessage *message);
Handle<Value> DecodeArguments(DBusMessage *message);

#endif
