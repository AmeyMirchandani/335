import asyncio
import bleak
import time

target_device = "Halo-Controller"

async def main():
    global gyroX, gyroY, gyroZ
    print("Searching for devices...")

    scanner = bleak.BleakScanner()
    await scanner.start()
    time.sleep(8)
    await scanner.stop()
    devices = scanner.discovered_devices

    for device in devices:
        if target_device == device.name:
            print(f"Found device: {device.name} with address: {device.address}")

            async with bleak.BleakClient(device.address) as client:
                print(f"Connected to {client.address}")
                print("Services Available:")

                services = client.services
                for num, service in enumerate(services):
                    print(f"Service UUID: {service.uuid}")
                    for chars in service.characteristics:
                        print(f"Characteristic UUID: {chars.uuid}")
                    print("--------------------")
                time.sleep(30)

if __name__ == "__main__":
    asyncio.run(main())