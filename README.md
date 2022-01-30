# "Realize" Project Template Utility

This is a simple command line utility that will create shell projects based on a template in ~/project-templates

## Prerequisites

* C compiler
* cmake


## Using

### Building 'realize'

1. Clone this repo to ~/ (this will be fixed later)
2. Make and install - `make install`

### Making your first scaffold

1. Create a project folder - `mkdir test1;cd test1`
2. Create Scaffold - `realize c myproj`
3. Build - `make`
4. Run - `build/myproj --help`

### Listing available templates

    realize --list


## Motivation

I create a lot of little programs to try things out, this reduces the boilerplate pain


## TODO

* make the project template root customizable
* improve the token substitution (should work in a streaming fashion)
* more project templates
