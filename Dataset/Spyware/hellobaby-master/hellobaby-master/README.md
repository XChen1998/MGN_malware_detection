# Hellobaby - Work in progress 


**The code in this repository is only for learning purposes, 
if you want to use it for other purposes you are doing it at 
your own risk ;)**


## Introduction

While studing how malwares work I tought that writing one on my own would be
a very good way to learn: *hellobaby* is my personal attempt to create a
polymorphic malware which would cover all the most common answers to the
question "What does a malware do?".

I will try to document very well every part of this project.

The principales goals are to get a better view on:

- how malwares spread
- how malwares get automatically re-executed
- how botnets work/communicate
- how modern malwares use to evade the most common detection techniques


For this *hellobaby* is composed by different modules, each of them
implementing a different technique to perform a certain task (e.g. spreading,
evasion techniques).

Yes, *hellobaby* is modular, this means you can choose which modules activate
and which not. 


## Writing a module - 

To write a new module you just need to follow few rules:
- put it in the appropriate section
- the name of the directory will be the name of the module, it can only
  contain alphanumeric caracters and underscores
- you need to define a function called `launch_<name of the module>`,
  it will be the entry point of your module
- keep your header files inside a folder include/, create an header file
  named `<name of the module>.h`. Your entry point function must be
  declared here
- after build, the compiled module must be a file inside the folder
  obj, named `<name of the module>.o`









