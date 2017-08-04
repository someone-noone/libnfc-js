{
  "targets": [
    {
      "target_name": "nfc-binding",
      "sources": [ "nfc.cc","nfc-device.cc", "addon.cc" ],
      "libraries": [ "-lnfc", "-L/usr/local/lib/", "-L/usr/lib/" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "/usr/local/include/",
        "/usr/include/"
      ]
    }
  ]
}
