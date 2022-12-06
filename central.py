import asyncio
import bleak
import time
import keyboard
import mouse
import struct

target_device = "Halo-Controller"

async def main():    
    
    print("Searching for devices...")

    scanner = bleak.BleakScanner()
    await scanner.start()
    time.sleep(5)
    await scanner.stop()
    devices = scanner.discovered_devices

    for device in devices:
        if target_device == device.name:
            print(f"Found device: {device.name} with address: {device.address}")

            async with bleak.BleakClient(device.address) as client:
                print(f"Connected to {client.address}")
                #print("Services Available:")

                services = client.services
                proxChar = services.get_characteristic("00002AA4-0000-1000-8000-00805f9b34fb")
                gyroXChar = services.get_characteristic("00002AA3-0000-1000-8000-00805f9b34fb")
                gyroYChar = services.get_characteristic("00002AA5-0000-1000-8000-00805f9b34fb")
                gyroZChar = services.get_characteristic("00002AA6-0000-1000-8000-00805f9b34fb")
                pitchChar = services.get_characteristic("00002AA1-0000-1000-8000-00805f9b34fb")
                rollChar = services.get_characteristic("00002AA2-0000-1000-8000-00805f9b34fb")

                await client.start_notify(proxChar, proxCallback)
                await client.start_notify(gyroXChar, gyroXCallback)
                await client.start_notify(pitchChar, pitchCallback)
                
                '''
                proxData = await client.read_gatt_char(proxChar)
                proxData = int.from_bytes(proxData, "big")
                print(f"Proximity Value: {proxData}")
                '''

                await asyncio.sleep(20)
            break

def printAvailableServicesInfo(services):
    for service in services:
        print("--------------------------------------")
        print(f"Service UUID: {service.uuid}")
        for chars in service.characteristics:
            print(f"Characteristic UUID: {chars.uuid}")

def pitchCallback(sender, data: bytearray):
    data = int.from_bytes(data, "little", signed=True)
    #print(data)

def gyroXCallback(sender, data: bytearray):
    data = struct.unpack("f", data)
    print(data[0])

def proxCallback(sender, data: bytearray):
    keyboard.send("f") #MUST FIX TO INCLUDE LEFT CLICK TOGGLE

if __name__ == "__main__":
    asyncio.run(main())