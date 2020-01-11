# I²C Callables

A small single-header Arduino based library that takes care of slave I²C management and function calls. [English README](#Purpose)  
Une petite bibliothèque Arduino à header unique qui gère l'I²C et l'appel de fonctions au travers de l'I²C côté esclave.

## Objectif

Le but de cette bibliothèque est de fournir un cadre pour gérer les communications I²C du côté d'un esclave, l'enregistrement de fonctions appelables par le maître ainsi que des fonctions partagées pour la manipulation des données.  
Elle fourni aussi deux fonctions côté maître pour simplifier l'envoi et la réception de données.

## Compatibilité

Pour le moment, la bibliothèque ne sera compatible qu'avec la bibliothèque Arduino Wire.


## Utilisation
### Esclave [🏠][Back to the top]

Du côté de l'esclave, l'utilisateur doit enregistrer des fonctions pour qu'elles soient appelables par le maître.
Cela est fait grâce à la fonction `registerRPC` qui permet d'enregistrer une fonction à l'indice demandé.  
Pour qu'une fonction puisse être enregistrée et appelée par le maître, elle doit avoir le prototype suivant : `BufferedData* f(BufferedData&)`.

Les fonctions à appeler doivent être créées par l'utilisateur.  
La struct `BufferedData` de retour doit être initialisée dans la fonction si besoin.Le constructeur prend la taille des données à stocker en argument.
Si la fonction ne retourne pas de données, retourner `nullptr`.

### Maître [🏠][Back to the top]

Le maître peut utiliser les fonctions `dataRequest` et `executeRPC` afin d'éxécuter une fonction sur l'esclave en récupérant, ou pas, les éventuelles données retournées.
Elles prennent toutes les deux un pointeur (optionnel) sur un `BufferedData` afin d'envoyer des arguments à l'esclave.  
Afin d'accéder à ces fonctions, la macro `I2CC_MASTER` doit être définie. Elle va aussi empêcher la compilation de fonctionnalités spécifiques aux esclaves.

L'appel de fonctions sur l'esclave est effectué simplement en envoyant l'indice de la fonction suivi de ses arguments, si elle en prend.  
Le message I²C est donc comme suit : `ID_Esclave ID_Fonction [Arguments ...]`, où les ID sont sur 8 bits.

Si la fonction appelée sur l'esclave renvoie quelque chose, la valeur de retour sera stockée sur l'esclave et sera envoyée au prochain appel de `Wire.requestFrom`.  
Si une autre fonction est appelée entre temps, la valeur de retour précédente sera effacée.

### Commun aux deux [🏠][Back to the top]

Des deux côtés de la communication, il est possible (et conseillé) d'utiser les fonctions `putData` et `getData` pour insérer ou récupérer des données d'un `BufferedData`.

#### ⚠️ Avertissements

 - Les deux fonctions incrémentent `BufferedData::cursor` et l'utilisent pour commencer le traitement. De ce fait, utiliser les deux fonctions sur le même `BufferedData` nécessite de faire un peu attention.
 - Du fait que le bourrage interne des structs dépend de l'implémentation, il est conseillé d'ordonner les types du plus gros au plus petit ou de n'utiliser qu'un seul type.
 - Faire attention au boutisme des deux microcontrôlleurs. Si il est différent, mettre l'argument `swapEndianness` de `putData` et `getData` à vrai.
 - La déduction des arguments template peut parfois promouvoir un type vers un type standard et causer des tailles inattendues. Par exemple, `uint8_t` sera déduit comme un `int` (qui peut être jusqu'à 4 fois plus gros!). Dans ce cas, il faut explicitement donner l'argument template. Par exemple : `putData<uint8_t>`.

## Exemples

Le code des exemples peut être trouvé dans le dossier `examples/` du dépôt.

 - Blinky distant : le maître demande périodiquement à l'esclave d'allumer ou d'éteindre sa LED, tout en la faisant clignoter de son côté aussi.
 - Compteur distant : le maître envoit un nombre à l'esclave. L'esclave va incrémenter ce nombre et le renvoyer au maître, qui va le stocker pour le renvoyer à la prochaine boucle.


Fin du README français.

-----

Start of the English README.

## Purpose [🏠][Back to the top]

The goal of this library is to provide a framework that handles the I²C communication as an I²C slave, the registration of functions callable by the master and unified data manipulation.  
It also provides two wrapper functions on the master side to handle data sending and receiving.

## Compatibility

For the time being, the library will only be compatible with the Arduino Wire library.

## Usage
### Slave [🏠][Back to the top]

On the slave side, the user has to register functions to be called by the master.
This is done via the `registerRPC` function which will take care of registering an existing function at the given index.  
The RPC methods need to match the following prototype : `BufferedData* f(BufferedData&)`

Function definition is left up to the user.  
Initialization of the `BufferedData` struct should be done in the function if it is needed. The constructor takes the size of the data to store as an argument.
If there is no return value, return `nullptr`.

The `startI2CC` needs to be called for the library to function, independently of the `registerRPC` calls. The method can return or loop indefinitely.

### Master [🏠][Back to the top]

The master can use the wrapper functions `executeRPC` and `dataRequest` to execute a function on the slave with or without retrieving what it returns.
Both take optional arguments to send to the slave via a `BufferedData`.  
To access those functions, the macro `I2CC_MASTER` must be defined. It will also prevent the compilation of slave-specific features.

Remote procedure call on the slave is realised by sending the function ID followed by its arguments, if it has any, to the slave.  
The I²C message is thus as follows : `Slave_ID RPC_ID [Arguments ...]` where both IDs are 8 bits long.

If the function returns some data, it will be stored and delivered on the next `Wire.requestFrom` call.  
If there is another write from the master in the meantime, the data will be destroyed.

### Both [🏠][Back to the top]

In both cases you can use the `putData` and `getData` functions to insert or extract data in or from a `BufferedData` object.  

#### ⚠️ Warnings

 - Both functions increment `BufferedData::cursor` and use it as the indicator to start processing. Thus, using both functions on the same `BufferedData` requires some extra attention.
 - As internal padding in structs is implementation dependant, try to order from largest to smallest types or use only one type.
 - Make sure both your microcontrollers use the same endianness. Set the `swapEndianness` argument of `putData` or `getData` to true if that's not the case.
 - Template deduction can sometimes use the standard type and cause issues. For example, `uint8_t` being deduced as an `int` (which can be as much as 4 times bigger!). In this case, explicitly use the template argument. For example : `putData<uint8_t>`.

## Examples

Example code can be found in the `examples/` folder of the repo.

 - Remote blinky : the master periodically asks the slave to turn its LED on or off, while blinking itself.
 - Remote counter : the master sends a number to the slave which it will increment and send it back to be sent over USB.

## TODO [🏠][Back to the top]

 - [x] Helper functions for parsing arguments
 - [x] ~~Macro for defining functions and adding them to the array~~ Function registering RPC methods in the array.
 - [ ] Error cases in an enum ?
 
 
 [Back to the top]: #IC-Callables
