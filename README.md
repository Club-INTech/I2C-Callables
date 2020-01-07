# I²C Callables

A small single-header Arduino based library that takes care of slave I²C management and function calls.

## Purpose

The goal of this library is to provide a framework that handles both the I²C communication as an I²C slave and the registration of callable functions that can return data to the master.

## Compatibility

For the time being, the library will only be compatible with the Arduino Wire library.

## Usage
### Slave

On the slave side, where the library is used, the user has to register functions to be called by the master.
This is done via the `registerRPC` function which will take care of registering an existing function at the given index.  
The RPC methods need to match the following prototype : `BufferedData* f(BufferedData&)`

Function definition is left up to the user.  
Initialization of the `BufferedData` struct should be done in the function if it is needed, `dataArray` should be malloc'd. Prefer using the provided constructor.
If there is no return value, return `nullptr`.

### Master

Remote procedure call on the slave is realised by "writing" a function ID followed by its arguments, if it has any, to the slave.  
The I²C message is thus as follows : `Slave_ID RPC_ID [Arguments ...]` where both IDs are 8 bits long.

If the function returns some data, it will be stored and delivered on the next `requestFrom()` call.  
If there is another write from the master in the meantime, the data will be destroyed.

### Both

In both cases you can use the `putData` and `getData` functions to insert or extract data in or from a `BufferedData` object.  
⚠️ Both functions increment `BufferedData::cursor` and use it as the indicator to start processing. Thus, using both functions on the same `BufferedData` requires some extra attention.

## TODO

 - [x] Helper functions for parsing arguments
 - [x] ~~Macro for defining functions and adding them to the array~~ Function registering RPC methods in the array.
 - [ ] Error cases in an enum ?
 