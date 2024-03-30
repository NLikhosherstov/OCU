# Build in Qt Creator
### Build page
_Build special stages_:
```
1. pio.exe pkg update
2. pio.exe -f -c qtcreator run
```

_Clean special stage_:
```
pio.exe -f -c qtcreator run --target clean
```

### Run page
_Deploy special stage_:
```
pio.exe run -t upload -e nanoatmega328
```

_Optional run special stage_:
```
pio.exe device monitor -b 115200
```
***check "Run in terminal"***
