import asyncio
import bleak
import time
import keyboard
import mouse

target_device = "Halo-Controller"
#global pitch, roll

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
                pitchChar = services.get_characteristic("00002AA1-0000-1000-8000-00805f9b34fb")
                rollChar = services.get_characteristic("00002AA2-0000-1000-8000-00805f9b34fb")

                await client.start_notify(proxChar, proxCallback)
                
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
    global pitch
    data = int.from_bytes(data, "little") 
    ## NEXT TO IMPLEMENT

def proxCallback(sender, data: bytearray):
    keyboard.send("f") #MUST FIX TO INCLUDE LEFT CLICK TOGGLE

if __name__ == "__main__":
    asyncio.run(main())