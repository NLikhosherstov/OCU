# Build in Qt Creator
### Build page
_Build special stages_
```
1. pio.exe pkg update
2. pio.exe -f -c qtcreator run
```

_Clean special stage_:
```
1. pio.exe -f -c qtcreator run --target clean
```

### Run page
_Deploy special stage_:
```
1. pio.exe run -t upload -e nanoatmega328
```

_Optional run special stage_:
```
1. pio.exe device monitor -b 115200
_check "Run in terminal"_ 
```
