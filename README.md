# EVSE INTERFACE


## Data Transfer Requests

### Get Alternative Measurements

Returns humidity.

```"method": "get_alt_measurements"```

E.g.:
```json
[2,"65329d79-7037-4d27-8405-7a48da77ba26","DataTransfer",{"vendorId":"1","data":"get_alt_measurements"}]
[3,"65329d79-7037-4d27-8405-7a48da77ba26",{"status":"Accepted","data":"temp:28.075533, hum:27.435822"}]
```

### Enable Debug Mode

Enables debug info on display and more verbose logs.

```"method": "enable_debug"```

E.g.:
```json
[2,"65329d79-7037-4d27-8405-7a48da77ba26","DataTransfer",{"vendorId":"1","data":"enable_debug"}]
[3,"65329d79-7037-4d27-8405-7a48da77ba26",{"status":"Accepted","data":""}]
```

### Disable Debug Mode

Disables debug info on display and more verbose logs.

```"method": "disable_debug"```


E.g.:
```json
[2,"65329d79-7037-4d27-8405-7a48da77ba26","DataTransfer",{"vendorId":"1","data":"disable_debug"}]
[3,"65329d79-7037-4d27-8405-7a48da77ba26",{"status":"Accepted","data":""}]
``` 


{
  "wifi": {
    "mode": 1,
    "ap": {
      "ssid": "vcon-????"
    },
    "sta": {
      "ssid": "Snaiper",
      "pass": "Snaiper401"
    }
  },
  "dash": {
    "url": "wss://dash.vcon.io/api/v3/rpc",
    "pass": "8GkHQZICha01zvRGqWliKqdup"
  },
  "debug": {
    "level": 2,
    "udpaddr": "",
    "tlslevel": 1
  },
  "device": {
    "id": "",
    "board": "",
    "dns": "",
    "provisioned": 1
  },
  "rpc": {
    "safe": ""
  },
  "host": {
    "arch": 211,
    "rst": 33,
    "mode": 1,
    "addr": 134217728,
    "boot0": 14
  },
  "uart": {
    "rx": 32,
    "tx": 34,
    "mode": 8,
    "baud": 115200,
    "crc": 0,
    "delim": "\n"
  },
  "spi": {
    "miso": -1,
    "mosi": -1,
    "clk": -1,
    "freq": 50000
  },
  "updi": {
    "pin": -1,
    "dur": 50
  },
  "swd": {
    "io": -1,
    "clk": -1,
    "dur": 50
  },
  "mqtt": {
    "url": "",
    "rx": "",
    "tx": "",
    "cert": "",
    "key": "",
    "hexdump": 0
  },
  "ws": {
    "url": "",
    "hdrs": "",
    "cert": "",
    "key": "/",
    "hexdump": 0
  },
  "ble": {
    "mode": 1,
    "filter": "",
    "lowat": 0
  },
  "eth": {
    "mode": 0,
    "addr": 0,
    "mdc": -1,
    "mdio": -1,
    "rst": -1,
    "phy": 0
  },
  "cell": {
    "mode": 0,
    "rx": -1,
    "tx": -1,
    "rst": -1,
    "baud": 115200,
    "at": "",
    "user": "",
    "pass": ""
  },
  "pins": {
    "factory": -1,
    "status": 16
  }
}