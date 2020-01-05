# I²C Callables

A small single-header Arduino based library that takes care of slave I²C management and function calls.

## Purpose

The goal of this library is to provide a framework that handles both the I²C communication as an I²C slave and the registration of callable functions that can return data to the master.

## Compatible chips

For the time being, the library will only be compatible with ATMega328P from Atmelt as it is the chip used by https://github.com/Club-INTech .

## Usage
### Slave

On the slave side, where the library is used, the user has to register functions to be called by the master.
This is done via the `TODO()` macro which will take care of declaring the function with a given name and store it at the requested ID.

Function definition is left up to the user.  
Initialization of the `ReturnData` struct should be done in the function if it is needed, `dataArray` should be malloc'd.
If there is no return value, return `nullptr`.

### Master

Remote procedure call on the slave is realised by "writing" a function ID followed by its arguments, if it has any, to the slave.  
The I²C message is thus as follows : `Slave_ID RPC_ID [Arguments ...]` where both IDs are 8 bits long.

If the function returns some data, it will be stored and delivered on the next `requestFrom()` call.  
If there is another write from the master in the meantime, the data will be destroyed.


## TODO

 - [ ] Helper functions for parsing arguments
 - [ ] Macro for defining functions and adding them to the array
 - [ ] Helper function for error cases (?)
 - [ ] Error cases in an enum ?
 