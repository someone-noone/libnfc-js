{
    "targets": [{
        "target_name": "nfc-binding",
        "sources": ["src/nfc.cc", "src/nfc-device.cc", "src/nfc-poll.cc", "src/addon.cc"],
        "libraries": ["-lnfc", "-L/usr/local/lib/", "-L/usr/lib/"],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "/usr/local/include/",
            "/usr/include/"
        ]
    }]
}
