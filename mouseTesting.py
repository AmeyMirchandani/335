from pynput import keyboard
from pynput.keyboard import Key, Controller
import asyncio

k = Controller()

def on_press(key):
    if key == Key.shift:
        k.press("w")
        k.release("g")

def on_release(key):
    if key == Key.shift:
        k.release("w")
        k.press("g")

listener = keyboard.Listener(on_press=on_press, on_release=on_release)
listener.start()