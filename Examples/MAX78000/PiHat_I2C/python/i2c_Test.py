import smbus
import time

MAX78000_Address = 0xAA

def poll_MAX78000(i2c):
       
    print("")
    print(f'Polling the MAX78000 at address {format(MAX78000_Address, "#04x")}...')
    try:
        i2c.read_byte(MAX78000_Address)
        print(f'Device detected at {MAX78000_Address}')
    except:
        print("No device found! Please verify PiHat connection and MAX78000 example firmware!")

    print("")


def sample_send(i2c):
    print("")
    print("Sending Sample Data to MAX78000")
    data = list()
    for i in range(0,32):
        data.append(i)
    try:
        i2c.write_block_data(MAX78000_Address,0,data)
        print("Success!")
    except:
        print("Failed to send test data!")


def sample_read(i2c):
    print("")
    print("Reading Sample Data from MAX78000")
    
    try:
        data_in = i2c.read_block_data(MAX78000_Address,MAX78000_SampleRead)
        print("Data Returned: [", end = "")
        for i in range(0,len(data_in)-1):
            print (f'{format(data_in[i], "#04x")}, ',end="")
        print(f'{format(data_in[-1], "#04x")}]')
    except:
        print("Failed to send test data!")

    print("")

def custom_command(i2c):
    print("")
    user_addr = int(input("Device Address (1-byte hex value, do NOT include '0x' prefix): "),16)
    user_cmd = int(input("Command (1-byte hex value, do NOT include '0x' prefix): "),16)
    user_data = input("Data (comma seperated bytes, no spaces, hex format, do NOT include '0x' prefix): ")
    data = list()
    if len(user_data) != 0:
        for byte in user_data.split(','):
            data.append(int(byte,16))
    print("")
    print("Custom Command: ")
    print(f'Device Address: {format(user_addr, "#04x")}')
    print(f'Command: {format(user_cmd, "#04x")}')
    if len(data) != 0:
        print("Data: [",end="")
        if len(data) > 1:
            for i in range(0,len(data)-1):
                print(f'{format(data[i], "#04x")},',end="")
        print(f'{format(data[-1], "#04x")}]')
    else:
        print("Data: []")
    print("")

    try:
        if len(data) > 0:
            i2c.write_block_data(user_addr,user_cmd,user_data)
        else:
            i2c.write_byte(user_addr,user_cmd)
        print("Succesful Write!")
    except:
        print("Failed to send custom command")
    print("")
    print("")

def custom_read(i2c):
    print("")
    user_addr = int(input("Device Address (1-byte hex value, do NOT include '0x' prefix): "),16)
    user_cmd = int(input("Command (1-byte hex value, do NOT include '0x' prefix): "),16)
    
    print("")
    print("Custom Read: ")
    print(f'Device Address: {format(user_addr, "#04x")}')
    print(f'Register Address: {format(user_cmd, "#04x")}')
    print("")

    try:
        data_in = i2c.read_block_data(user_addr,user_cmd)
        print("Data Returned: [", end = "")
        for i in range(0,len(data_in)-1):
            print (f'{format(data_in[i], "#04x")}, ',end="")
        print(f'{format(data_in[-1], "#04x")}]')
    except:
        print("Failed to read specified register")
    print("")
    print("")

def main():


    i2c = smbus.SMBus(1)

    menu_options = {    '1' : poll_MAX78000,
                        '2' : sample_send,
                        '3' : sample_read,
                        '4' : custom_command,
                        '5' : custom_read,
    }

    print("*** MAX78000 PiHat I2C example program ***")
    print("")
    print("Please ensure that the MAX78000 PiHat is properly")
    print("connected to RPI, and the example firmware has")
    print("flashed to the MAX78000.")
    print("")
    #Run until user exits program
    while True:
        
        print("Please select one of the options below:")
        print("(1) Poll I2C bus for MAX78000")
        print("(2) Send Sample Data to MAX78000")
        print("(3) Read Sample Data from MAX78000")
        print("(4) Custom Write Command")
        print("(5) Custom Read Command")
        
        user_selection = input("")

        if user_selection in menu_options:
            menu_options[user_selection](i2c)
        else:
            print("Entry not recognized, please try again")


        #Small delay between transmissions
        time.sleep(0.1)


if __name__ == "__main__":

    #Call main function
    main()