{
  "targets": [
    {
      "target_name": "nativemodule",
      "sources": [ "nativemodule.c" ],
      "include_dirs": [
        "../3rdparty/src/libobs"
      ],
      "libraries": [
        "../3rdparty/lib/liblibobs-version.a",
        "../3rdparty/lib/libobs.a",
      ],
      "cflags": ["-DOB_BUILD"]
    }
  ]
}
