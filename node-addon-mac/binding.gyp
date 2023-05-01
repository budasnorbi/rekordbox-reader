{
  "targets": [
    {
      'target_name': 'rekordbox-reader',
      'sources': [
        'rekordbox-reader.cpp'
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
        ]      
      }
  ]
}
