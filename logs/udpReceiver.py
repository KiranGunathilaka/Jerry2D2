import socket

host = '192.168.1.53'
port = 1234
data_string = ''
data_array = [0.0,0.0,0,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0] #0distance/1anle/2 left rps/3 right rps/4 gyro x/5 gyro y/6 gyro z/7 tof left/8 tof centre/9 tof right/10 magnetometer directio

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((host, port))

print("Listening on {}:{}".format(host, port))


while True:
    data, addr = sock.recvfrom(1024)
    data_dec = data.decode().split(":")
    if (data_dec[0] == '/'):
        print(data_array)
        print(data_string)
        data_string = ''
    try:
        data_string += data_dec[0]+data_dec[1]
    except:
        pass
    match data_dec[0]:
        case "Distance":
            data_array[0]=data_dec[1]
        case "Angle":
            data_array[1]=data_dec[1]
        case "Left RPS":
            data_array[2]=data_dec[1]
        case "Right RPS":
            data_array[3]=data_dec[1]
        case "Gyro X":
            data_array[4]=data_dec[1]
        case "Gyro Y":
            data_array[5]=data_dec[1]
        case "Gyro Z":
            data_array[6]=data_dec[1]
        case "ToF Left":
            data_array[7]=data_dec[1]
        case "ToF Center":
            data_array[8]=data_dec[1]
        case "ToF Right":
            data_array[9]=data_dec[1]
        case "Direction":
            data_array[10]=data_dec[1]
        case "/":
            pass        #data_array[0]=
    
