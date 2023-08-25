import spidev
import time


def sample_transaction(spi):
    print("")
    print("Sample Transaction")
    print("Sending 0-31...")
    sample_data = list()
    for i in range(0,32):
        sample_data.append(i)
    print(f'User Sent:     {sample_data}')
    rcvd = spi.xfer(sample_data)
    print(f'User Received: {rcvd}')
    print("Sample transmission complete!")
    print("")


def custom_transaction_ascii(spi):
    print("")
    print("Custom Transaction (ASCII)")
    user_input = []
    user_input = input("Please type an ASCII message to send over the SPI Bus:")
    print(f'User Sent:     {list(user_input.encode())}')
    rcvd = spi.xfer((user_input.encode()))
    print(f'User Received: {rcvd}')
    print("")

def custom_transaction_hex(spi):
    print("")
    print("Custom Transaction (HEX)")
    user_input = []
    print("Please type a HEX message to send over the SPI Bus:")
    user_input = input("Seperate values with commas, and do not include '0x':")
    user_data = list()
    if len(user_input) != 0:
        user_input.strip(" ")
        for byte in user_input.split(','):
            user_data.append(int(byte,16))
    print(f'User Sent:     {user_data}')
    rcvd = spi.xfer(user_data)
    print(f'User Received: {rcvd}')
    print("")


def main():
    #Select SPI bus # (Bus 0) and Chip Select (Select pin 0)
    spi = spidev.SpiDev(0,0)

    #Set max speed to 1 MHz
    spi.max_speed_hz = 1000000

    menu_options = {    '1' : sample_transaction,
                        '2' : custom_transaction_ascii,
                        '3' : custom_transaction_hex
    }

    #Run until user exits program
    while True:
        print("*** MAX78000 PiHat SPI example program ***")
        print("Please select one of the options below:")
        print("(1) Sample Transaction")
        print("(2) Custom Transaction (ASCII Input)")
        print("(3) Custom Transaction (Hex Input)")
        
        user_selection = input("")

        if user_selection in menu_options:
            menu_options[user_selection](spi)
        else:
            print("Entry not recognized, please try again")

        #Small delay between transmissions
        time.sleep(0.1)


if __name__ == "__main__":

    #Call main function
    main()