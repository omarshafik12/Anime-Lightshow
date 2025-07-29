"""
Notes

#Status has three modes to organize and dictate actions based on stages, got bored with the names:
    #Eddard Stark, means appending to file is fine
    #Joffery, means take the file with its contents and send to TinyML, and delete file
    #Ice King, means to discard all new data coming in until OpenAI sends something 
    #Bran, means you send results back, make a new file and start over

#writing to file, contingent on if tinyML or OpenAI is running processes or not
#if so, data is ignored and discared

#results returned will just be a string saying the name of the anime or None
"""


import socket
import os

#creating file, removing in case program crashed and left file with or without data
if os.path.exists("audio_file.wav"):
  os.remove("audio_file.wav")
else:
  print("creating file")
  open("audio_file.wav", "w").close()

#Server sets up a listening Socket
HOST = "127.0.0.1" #in the future you would have a more realistic way of doing it, hard coded for simplicity
PORT = 8000
bytes_so_far = None
STATUS = "Eddard Stark"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()

#Reading what we receive
    with conn:
        print (f"Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break

            if STATUS == "Eddard Stark":
                with open("audio_file.wav", "ab") as file:
                    file.write(data)
                #checks if file has 5 seconds worth of audio
                bytes_so_far += len(data)
                if bytes_so_far >= 441000: STATUS = "Joffery"

            elif STATUS == "Joffery":
                #TinyML send
                print("hi")

                #if music send to Whisper and OpenAI and change, else set to Bran and continue
            
            elif STATUS == "Ice King":
                #wait for openAI confirmation
                print("hi")
            
            elif STATUS == "Bran":
                #Remake file
                if os.path.exists("audio_file.wav"):
                    os.remove("audio_file.wav")
                else:
                    print("creating file")
                    open("audio_file.wav", "w").close()

                #Send results back to the client

                #return status to eddard stark
                STATUS = "Eddard Stark"