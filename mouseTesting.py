from pynput import keyboard
from pynput.keyboard import Key, Controller
import asyncio

k = Controller()

def on_press(key):
    if key == Key.shift:
        k.press("f")

def on_release(key):
    if key == Key.shift:
        k.release("f")

listener = keyboard.Listener(on_press=on_press, on_release=on_release)
listener.start()