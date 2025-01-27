# ![cmount_logo](.github/images/cmount.png)
Unifying application for efficient and C-dependent 
program development.

Cmount is a debug and builder application 
to make the developers life easier. It provides a 
standardized project structure, dependency management 
and hot reload for all supported languages in the 
development cycle through a llvm just in time compiler.

## Cross-platform
Cmount comes with C, Python and Dart on the development 
platforms Windows, macOS and Linux as well the target platforms
Windows, macOS, Linux, android, iOS.
It can cross compile to target mobile platforms.

## Hot reload
Cmount has hot reload for C and Dart as languages. But every
language can be added with such a feature from Cmount's side.

## Easy project configuration
Because Cmount is heavily inspired by the Flutter command line tool,
you will find a similar idea of project configuration.

`spec.yaml`
```yaml
name: project_name
version: dev-.1 # Strings are allowed.
description: sample description
authors:
...
license: license.md # Files always relative to project root.

c:
  type: shared # Available: shared, executable
  sources: src/ # It will be scanned for .c files.
  standard: C23 # ISO standards.
  
dart: 
  type: native # Available: native, package
  sources: src/
  version: 3.4.1
  pub: # Dart pub package manager dependencies.
    yaml: ^2.3
    ...
  ...
```
But you maybe notice, a project has its limitations about
multiple executables etc. For specific use cases you definitely
should consider to build an extension.

```json
{
  "name": "extension_name",
  "description": "sample description",
  "depend": [
    "Python"
  ],
  "link": "import",
  "bundle": {
    "platforms": {
      "all": [
        "config.yaml"
      ],
      "windows": [
        "sample.win.dll"
      ]
    },
    ...
  }
}
```

> 
## Unit protocol
Cmount comes with an API to extend it. But the special feature about it
is, the extensions you write will not only be able to communicate with Cmount, 
but also with applications built with Cmount.

This is such a great feature because it allows applications built with 
Cmount to seamlessly integrate modding capabilities. All extensions written 
for Cmount are also extensions for Cmount based applications that use 
the protocol.

You can decide if you want your app to be extended/modded or not.

## Participated components
Cmount is created for **Fireworks**, a graphic engine framework.
As result, Cmount is actually intended to be extended and is participated
in its components as shared libraries.
