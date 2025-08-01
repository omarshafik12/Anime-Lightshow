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
import wave
import os
import whisper
import openai
from dotenv import load_dotenv

#creating file, removing in case program crashed and left file with or without data
if os.path.exists("audio_file.raw"):
  os.remove("audio_file.raw")
else:
  print("creating file")
  open("audio_file.raw", "wb").close()

#loading whipser model
model = whisper.load_model("base")

#Server sets up a listening Socket
HOST = "127.0.0.1" #in the future you would have a more realistic way of doing it, hard coded for simplicity
PORT = 8000
bytes_so_far = 0
status = "Eddard Stark"

#Wave file parameters
SAMPLE_RATE   = 44100
SAMPLE_WIDTH  = 2
CHANNELS      = 1

#accessing openai gpt-4o model to dechiper the present anime
load_dotenv() 
OPENAI_API_KEY = os.getenv("OPENAI_API_KEY")
def figure_out_anime(prompt):
    response = openai.ChatCompletion.create(
        model = "gpt-4o",
        messages = [{"role":"user", "content": f"Based of the lyrics of below, please only return the string, One Piece, Naruto, My Hero Academia if it matches the lyrics of one of their anime openings, else return just the string None. Lyrics: {prompt}"}]
    )

    return response.choices[0].message.content.strip()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()

#Reading what we receive
    with conn:
        print (f"Connected by {addr}")
        while True:
            data = conn.recv(36000)
            if not data:
                break

            if status == "Eddard Stark":
                with open("audio_file.raw", "ab") as file:
                    file.write(data)
                #checks if file has 5 seconds worth of audio
                bytes_so_far += len(data)
                if bytes_so_far >= 441000: status = "Joffery"

            elif status == "Joffery":
                #transfer raw bytes into WAV file, 
                with open("audio_file.raw", 'rb') as pcmfile:
                    pcmdata = pcmfile.read()
                    nframes = len(pcmdata) // (SAMPLE_WIDTH * CHANNELS)
                with wave.open('audio_file.wav', 'wb') as wavfile:
                    wavfile.setparams((CHANNELS,
                       SAMPLE_WIDTH,
                       SAMPLE_RATE,
                       nframes,
                       "NONE", "NONE"))
                    wavfile.writeframes(pcmdata)
                if os.path.exists("audio_file.raw"):
                    os.remove("audio_file.raw")

                #TinyML send
                print("hi")
                """
                use a boolean variable
                If TinyML says this is music we send this file to OpenAI by setting status to Ice King
                If it is not music then we simply just go to BRAN and restart
                """
            
            elif status == "Ice King":
                #wait for openAI confirmation
                # load audio and pad/trim it to fit 30 seconds
                audio = whisper.load_audio("audio_file.wav")
                audio = whisper.pad_or_trim(audio)

                # make log-Mel spectrogram and move to the same device as the model
                mel = whisper.log_mel_spectrogram(audio, n_mels=model.dims.n_mels).to(model.device)

                # detect the spoken language
                _, probs = model.detect_language(mel)
                print(f"Detected language: {max(probs, key=probs.get)}")

                # decode the audio
                options = whisper.DecodingOptions()
                result = whisper.decode(model, mel, options)

                #result.text is what you send to openai or append to string that you send to openai
                answer = figure_out_anime(result.text)

                #resetting 
                status = "Bran"
            
            elif status == "Bran":
                #Remake file
                if os.path.exists("audio_file.wav"):
                    os.remove("audio_file.wav")
                print("creating file")
                open("audio_file.raw", "wb").close()

                #Send results back to the client

                #return status to eddard stark
                bytes_so_far = 0
                status = "Eddard Stark"
"""
Resources:
    - Wave header
        - https://stackoverflow.com/questions/16111038/how-to-convert-pcm-files-to-wav-files-scripting/16111188

    - Whisper prereqs:
        - https://github.com/openai/whisper
        - Need to pip install whipser and download ffmpeg, and my need to install rust as well.
"""