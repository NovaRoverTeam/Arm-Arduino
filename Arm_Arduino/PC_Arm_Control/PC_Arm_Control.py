import serial
import time
from pynput import keyboard

arduinoData = serial.Serial('COM10', 2000000, writeTimeout = 0)
time.sleep(2)

def led_on():
    arduinoData.write(bytes(b'light_on\n'))
    print('on')


def led_off():
    arduinoData.write(bytes(b'light_off\n'))
    print('off')



flag = [False]*26

def n(letter):
    return ord(letter) - ord('a')

def on_press(key):
    global flag
    try:
        if (not flag[n('q')]) and key.char == 'q':
            arduinoData.write(bytes(b'base,1\n'))
            flag[n('q')] = True
        elif (not flag[n('a')]) and key.char == 'a':
            arduinoData.write(bytes(b'base,-1\n'))
            flag[n('a')] = True
        elif (not flag[n('w')]) and key.char == 'w':
            arduinoData.write(bytes(b'actuator_lower,1\n'))
            flag[n('w')] = True
        elif (not flag[n('s')]) and key.char == 's':
            arduinoData.write(bytes(b'actuator_lower,-1\n'))
            flag[n('s')] = True
        elif (not flag[n('e')]) and key.char == 'e':
            arduinoData.write(bytes(b'actuator_upper,1\n'))
            flag[n('e')] = True
        elif (not flag[n('d')]) and key.char == 'd':
            arduinoData.write(bytes(b'actuator_upper,-1\n'))
            flag[n('d')] = True
        elif (not flag[n('r')]) and key.char == 'r':
            arduinoData.write(bytes(b'wrist_horizontal,1\n'))
            flag[n('r')] = True
        elif (not flag[n('f')]) and key.char == 'f':
            arduinoData.write(bytes(b'wrist_horizontal,-1\n'))
            flag[n('f')] = True
        elif (not flag[n('t')]) and key.char == 't':
            arduinoData.write(bytes(b'wrist_vertical,1\n'))
            flag[n('t')] = True
        elif (not flag[n('g')]) and key.char == 'g':
            arduinoData.write(bytes(b'wrist_vertical,-1\n'))
            flag[n('g')] = True
        elif (not flag[n('y')]) and key.char == 'y':
            arduinoData.write(bytes(b'wrist_rotate,1\n'))
            flag[n('y')] = True
        elif (not flag[n('h')]) and key.char == 'h':
            arduinoData.write(bytes(b'wrist_rotate,-1\n'))
            flag[n('h')] = True
        elif (not flag[n('u')]) and key.char == 'u':
            arduinoData.write(bytes(b'effector_angle,1\n'))
            flag[n('u')] = True
        elif (not flag[n('j')]) and key.char == 'j':
            arduinoData.write(bytes(b'effector_angle,-1\n'))
            flag[n('j')] = True
        elif (not flag[n('i')]) and key.char == 'i':
            arduinoData.write(bytes(b'effector_position,1\n'))
            flag[n('i')] = True
        elif (not flag[n('k')]) and key.char == 'k':
            arduinoData.write(bytes(b'effector_position,-1\n'))
            flag[n('k')] = True
        elif (not flag[n('o')]) and key.char == 'o':
            arduinoData.write(bytes(b'light,1\n'))
            flag[n('o')] = True
        elif (not flag[n('l')]) and key.char == 'l':
            arduinoData.write(bytes(b'light,-1\n'))
            flag[n('l')] = True
    except AttributeError:
        print('')



def on_release(key):
    global flag
    if key.char == 'q' or key.char == 'a':
        arduinoData.write(bytes(b'base,0\n'))
        flag[n('q')] = False
        flag[n('a')] = False
    if key.char == 'w' or key.char == 's':
        arduinoData.write(bytes(b'actuator_lower,0\n'))
        flag[n('w')] = False
        flag[n('s')] = False
    if key.char == 'e' or key.char == 'd':
        arduinoData.write(bytes(b'actuator_upper,0\n'))
        flag[n('e')] = False
        flag[n('d')] = False
    if key.char == 'r' or key.char == 'f':
        arduinoData.write(bytes(b'wrist_horizontal,0\n'))
        flag[n('r')] = False
        flag[n('f')] = False
    if key.char == 't' or key.char == 'g':
        arduinoData.write(bytes(b'wrist_vertical,0\n'))
        flag[n('t')] = False
        flag[n('g')] = False
    if key.char == 'y' or key.char == 'h':
        arduinoData.write(bytes(b'wrist_rotate,0\n'))
        flag[n('y')] = False
        flag[n('h')] = False
    if key.char == 'u' or key.char == 'j':
        arduinoData.write(bytes(b'effector_angle,0\n'))
        flag[n('u')] = False
        flag[n('j')] = False
    if key.char == 'i' or key.char == 'k':
        arduinoData.write(bytes(b'effector_position,0\n'))
        flag[n('i')] = False
        flag[n('k')] = False
    if key.char == 'o' or key.char == 'l':
        arduinoData.write(bytes(b'light,0\n'))
        flag[n('l')] = False
        flag[n('o')] = False
    if key == keyboard.Key.esc:
        # Stop listener
        return False


# Collect events until released
with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()


