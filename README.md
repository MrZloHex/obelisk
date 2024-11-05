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