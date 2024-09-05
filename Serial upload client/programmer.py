import sys
import os
import time
import serial
import serial.tools.list_ports as portLister

EEPROM_SIZE_BYTES = 32768
BLOCK_COUNT = 4


class FileSizeError(Exception):
    pass

class FileTypeError(Exception):
    pass


def help():
    """Called when the user runs 'python programmer.py help'
    """
    print("help function still to be written")


def readBinary(fileName):
    """Checks that the binary file is valid, then reads it in blocks

    Args:
        fileName (str): the name of the binary file to be read

    Raises:
        FileSizeError: if the file does not match the expected size
        FileNotFoundError: if the file does not exist
        
    Returns:
        dataBlocks: a list of bytes objects formed from the input file
    """
    try:
        fileSize = os.stat(fileName).st_size
        if fileSize != EEPROM_SIZE_BYTES:
            raise FileSizeError(fileSize)
        
        with open(fileName, "rb") as binFile:
            dataBlocks = []
            for i in range(BLOCK_COUNT):
                dataBlocks.append(binFile.read(EEPROM_SIZE_BYTES//BLOCK_COUNT))
                
    except FileSizeError as e:
        print(f"Provided file is {e} bytes long, expected {EEPROM_SIZE_BYTES} bytes. Quitting...")
        sys.exit(1)
    except FileNotFoundError:
        print("The provided file does not exist. Make sure it is in the same directory as this program. Quitting...")
        sys.exit(1)
    
    return dataBlocks
        
    



def chooseCOMPort():
    """Display the open COM ports and allow the user to choose one

    Returns:
        ListPortInfo: the port information of the port that the user selected
    """
    ports = {i.device:i for i in list(portLister.comports())}
    choice = ""
    while "COM" + choice not in ports.keys():
        print("Please choose one of the following ports:")
        for k,v in ports.items():
            print(f"-  {k} ({v.description})")
        choice = input(">COM")
    print(f"COM{choice} selected successfully.")
    return "COM"+choice


def openCOMPort(portName):
    """Attempt to connect to the selected COM port with a suitable baud rate and timeout

    Args:
        portName (str): the name of the COM port (will be "COM" followed by a number)

    Returns:
        serial.Serial: the serial port connection object from pySerial
    """
    try:
        arduino = serial.Serial(port='COM5',  baudrate=115200, timeout=.5)
    except Exception as e:
        print("Error opening COM port. Quitting...")
        print(e)
        sys.exit(1)
        
    return arduino


def establishConnection():
    """Establish the connection with the arduino by sending a byte and waiting for a response.

    Raises:
        TimeoutError: if there is no response from the arduino in the given time limit
        ValueError: if the arduino responds incorrectly

    Returns:
        bool: true if the connection was successful, false if not
    """
    arduino = serial.Serial(port='COM5',  baudrate=115200, timeout=.5)
    time.sleep(.05)
    arduino.write(bytes([255]))
    time.sleep(.05)
    resp = arduino.read(1)
    try:
        print(resp)
        if len(resp) != 1:
            raise TimeoutError()
        if resp != b'\xaa':
            raise ValueError()
        else:
            print("AA recieved.")
            return True
    except TimeoutError:
        print("Timeout reached waiting for response. Please restart the arduino and try again. Quitting...")
        arduino.close()
        sys.exit(1)
    except ValueError:
        print("Incorrect response recieved. Please restart the arduino and try again. Quitting...")
        arduino.close()
        sys.exit(1)
    

def sendBlocks(portObject, dataBlocks):
    
    return True


def main():
    if len(sys.argv) == 2:
        if sys.argv[1] == "help":
            help()
        elif sys.argv[1].endswith(".bin"):
            dataBlocks = readBinary(sys.argv[1])
            selectedPort = chooseCOMPort()
            #arduino = openCOMPort(selectedPort)
            establishConnection()
            #sendBlocks(arduino, dataBlocks)
            #arduino.close()
            
                
            
            
            
    else:
        print("Usage: python programmer.py <inputfile>. Type 'python programmer.py help' for more info.")




if __name__ == "__main__":
    main()