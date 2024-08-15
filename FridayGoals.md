# Backend Team Goals
Hey guys, Chiemeka here. This file here serves as a benchmark that any of the backend team members can reference at any time if they're confused what my commits may be working towards. I've tried to outline the steps I think are necesssary for a basic level of functionality within the build project and luckily they're a lot less involved than I thought.
I've also decided that all the user profile stuff with feeding schedules, last time fed, etc etc will be shelved until the shit works so lets just ignore the mongodb connection for now. 

[Here](https://github.com/lamluongg/PetFeederApp) is what the frontend guys are working on, so we don't get lost and come up with a backend that is a complete clusterfuck of spaghetti code bullshit)

# What is this plan based on?
This plan is based on what i've been able to find regarding the specific parts the electronic people are trying to use. if u guys have better/simpler ideas for anything here feel free to update this goal sheet and the rest of the backend people because this shit is all new 2 me too

[Arduino](https://docs.arduino.cc/hardware/uno-r4-wifi/)

[Motors](https://www.amazon.com/180%C2%B0Metal-Waterproof-Airplane-Helicopter-Mechanical/dp/B09JWK2GB3/ref=asc_df_B09JWK2GB3/?tag=hyprod-20&linkCode=df0&hvadid=693372426412&hvpos=&hvnetw=g&hvrand=13680442756324950130&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9030950&hvtargid=pla-1675722646259&mcid=c8149b27222b305fba9ddb599ece2ec9&gad_source=1&gclid=CjwKCAjw9IayBhBJEiwAVuc3fuO7w9rWWalcVut3Rp_iFuyFVI67w19VCnRrRjHwNZaiEYqhCxGJFhoCOLMQAvD_BwE&th=1)

[Ultrasonic Distance Sensor](https://www.amazon.com/180%C2%B0Metal-Waterproof-Airplane-Helicopter-Mechanical/dp/B09JWK2GB3/ref=asc_df_B09JWK2GB3/?tag=hyprod-20&linkCode=df0&hvadid=693372426412&hvpos=&hvnetw=g&hvrand=13680442756324950130&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9030950&hvtargid=pla-1675722646259&mcid=c8149b27222b305fba9ddb599ece2ec9&gad_source=1&gclid=CjwKCAjw9IayBhBJEiwAVuc3fuO7w9rWWalcVut3Rp_iFuyFVI67w19VCnRrRjHwNZaiEYqhCxGJFhoCOLMQAvD_BwE&th=1) (this isn't included in the skeleton code yet)

[Water Sensor](https://www.amazon.com/DIYables-Detector-Arduino-ESP8266-Raspberry/dp/B0BXKMLB4D/ref=sr_1_4?dib=eyJ2IjoiMSJ9.ZVIioIY-FafGAb2kIcXift6MrXgen7D2dt1ZqbB82NjF7jyGlvDS_SvIOkCjFV9LlE4LntB1AEagxJODWjjMXRGIH_jNKi0UG5ez-ULLyeBnCN1Q_7FL0xxf5if6CTbdDKj7A-T0QGIC9-wcQzgn909aXC_Pst5wulv62HdnMMZ0_qmKoMxgOC-1bx_MhjY_nCFvBwFSjs6yxMPC3ukY3s4lYz6AReFu_A5RPdBVGvA.4p2_XUmw421m3oTTJ2PyK-l7D0en_Wyotac9A1pA-vA&dib_tag=se&keywords=water+level+sensor+arduino&qid=1715631482&sr=8-4) (this isn't included in the skeleton code yet)


## Objective
Establish a basic web server on the Arduino and ensure two-way communication between the web page and the Arduino. Our success metric for this will be being able to move the motors remotely from a web page. 

### Key Goals

#### 1. Set Up a Basic Web Server on Arduino
- **Description**: Implement a simple web server that can handle HTTP requests on the Arduino.
- **Tools**:
  - Arduino IDE
  - Arduino ESP32 Board (this is the wifi chip on the arduino uno rev4 with wifi, so don't worry about that)
  - WiFi library and Web Server library for Arduino ([I found exactly how to do that here](https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/))
  - the arduino points look something like [lines 9 through 20 in this file](https://github.com/ChiemekaKalu/pet-feeder-build-project-backend/blob/dev/arduino.ino)
- **Steps**:
  1. Install the necessary board and libraries using the arduino IDE/library manager (we just need the board in hand to do this)
  2. Configure the Arduino to connect to the local WiFi network. (this will be complicated because i dont know how friendly UCLA's network is to IoT devices)
  3. Implement HTTP GET and POST handlers to receive and respond to web-based requests. (very basic endpoints we got that in the bag)

#### 2. Web Page and Arduino Communication
- **Description**: Develop a mechanism for the web page to send commands to the Arduino and receive responses.
- **Tools**:
  - HTML/JavaScript for the frontend (u goats got that)
  - Arduino programming for the backend (this is almost done, i already wrote the skeleton code for this)
- **Steps**:
  1. Design a simple web interface with buttons and info display
  2. Implement (AJAX?, don't really know how this one works) calls in the web page to interact with the Arduino's web server.
  3. Setup Arduino to parse incoming requests and send appropriate responses (this thing basically just needs to reply with "ok" or "can't)

#### 3. Testing and Validation
- **Description**: Ensure that this shit works well for PM/Friday
- **Tools**:
  - Web browser
  - Serial Monitor (this won't really be possible unless we can connect a cable out of the box to a laptop to monitor the serial but its whatever fucc it. more on this later)
- **Steps**:
  1. Conduct manual tests by interacting with the web interface and observing the Arduino's behavior. (ez)
  2. Monitor the serial output from Arduino for debugging and validation. (if possible ez)
  3. Implement error handling in both Arduino code and web page JavaScript to manage communication failures. (ehhhhh error handling is for pussies)

### Milestones
- [ ] Web server setup on Arduino completed by end of Thursday
- [ ] Two-way communication established and initial testing done by end of lockin
- [ ] Final testing done, being able to add those bs features afterwards 

### what we need right now to do any more
- Arduino board (esp32). we need the board to upload the code to it and start testing our web shit
- Basic web hosting setup, gon need to do some research on this, this should just be fine on localhost for now for now though but we gonna need a free host or pay for a cheap host by the time we done though

### Risks and Mitigation 
  -**risk** this is all in theory!
 
  --**mitigation**: we will test it as best we can, but this is why i want to keep the functionality ***SUPER*** simple at this moment 

## Conclusion
we have a lot to do, but its not too much more complicated than a basic client server communication and we got it in the bag lol.

