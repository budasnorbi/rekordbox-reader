{
  "targets": [
    {
      'target_name': 'callback_addon',
      'sources': [
        'callback_addon.cpp'
      ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
        "defines": [
          "_GNU_SOURCE"
        ],
        "cflags!": [
          "-fno-exceptions"
        ],
        "cflags_cc!": [
          "-fno-exceptions"
        ],
        "xcode_settings": {
          "OTHER_CFLAGS": [
            "-fno-exceptions"
          ],
          "OTHER_CPLUSPLUSFLAGS": [
            "-fno-exceptions"
          ]
        }
      }
  ]
}
