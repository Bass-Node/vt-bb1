# -*- coding: utf-8 -*-

import time
import bluetooth
from wakeonlan import

phone = "ff:ff:ff:ff:ff:ff"

def search():
    devices = bluetooth.discover_devices(duration = 5, lookup_names = True)
    return devices

while True:
    results = search()

    for addr, name in results:
        if addr == phone:
            wol.send_magic_packet('ff:ff:ff:ff:ff:ff')

        time.sleep(20)