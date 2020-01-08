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

The `startI2CC` needs to be called for the library to function, independently of the `registerRPC` calls. The method can return or loop indefinitely.

### Master

Remote procedure call on the slave is realised by "writing" a function ID followed by its arguments, if it has any, to the slave.  
The I²C message is thus as follows : `Slave_ID RPC_ID [Arguments ...]` where both IDs are 8 bits long.

If the function returns some data, it will be stored and delivered on the next `requestFrom()` call.  
If there is another write from the master in the meantime, the data will be destroyed.

### Both

In both cases you can use the `putData` and `getData` functions to insert or extract data in or from a `BufferedData` object.  

#### ⚠️ Warnings

 - Both functions increment `BufferedData::cursor` and use it as the indicator to start processing. Thus, using both functions on the same `BufferedData` requires some extra attention.
 - As internal padding in structs is implementation dependant, try to order from largest to smallest types or use only one type.
 - Make sure both your microcontrollers use the same endianness. Set the `swapEndianness` argument of `putData` or `getData` to true if that's not the case.
 - Template deduction can sometimes use the standard type and cause issues. For example, `uint8_t` being deduced as an `int` (which can be as much as 4 times bigger!). In this case, explicitly use the template argument. For example : `putData<uint8_t>`.

## TODO

 - [x] Helper functions for parsing arguments
 - [x] ~~Macro for defining functions and adding them to the array~~ Function registering RPC methods in the array.
 - [ ] Error cases in an enum ?
 