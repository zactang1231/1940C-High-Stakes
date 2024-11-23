<hr>

# 1940C Burnout - Nationals Repo 🔥

Hi! You've stumbled upon the official 1940C Burnout repository for the 2024 Australian National Championships.
We'll use this for our Nationals code and we'll frequently update it as we develop our program. For our States code, visit our States repository.

btw pls **star** our project if you want to support us :)

---

## Who are we?

### Team Description

**1940C Burnout** is a primarily second-year VEX V5RC team. Qualifying for Middle School but playing up to the High School division, we share a passion for robotics and technology. Driven by a commitment to excellence, we are determined to leave our mark in this season's VEX competition and in future seasons as well.

### Roles

**Zachary Tang** - main scout, main strategist, main designer  
**Jasper Liu** - main builder, secondary scout  
**William Hill** - main notebooker, main admin  
**William Headley** - main programmer, secondary builder, secondary designer  
**Andy Jiang: Driver** - secondary builder  

---

## Where do I find everything?

Below is an overview of our repository, mostly just the important stuff

```
1940C-NATIONALS/
├── include/            # all of our header files (.h, .hpp)
│  
├── src/                # all of the source files (.cpp)
│   ├── main.cpp            # main file, kept as clean as possible 
│   └── robot-config.cpp    # configuring the robot (motors, odom, pid, etc.)
│
└── README.md           # what you're reading right now
```

---

## What do I do with this?

To run our code, you first need to:
1. Install the PROS API (https://pros.cs.purdue.edu/)
    - *Do this through the VSCode Extension*
2. Set up the Brain, Controller and Robot 
    - *Make sure all the motors, ports and other components are in accordance with the code*

Then, you can use our code by:
1. Cloning the repository

```bash  
git clone https://github.com/whatever-your-username-is/1940C-Nationals.git
```  

2. Open in your editor
  - *Probably VSCode since we're using PROS*
3. Build and Upload the code
  - *There are a few ways to do this:*  
      a) Use ```pros mu``` in your terminal  
      b) Click on the PROS icon in your left sidebar, then PROS > Quick Actions > Build & Upload  
      c) Click on the PROS icon on your top right (this is a handy shortcut for Build and Upload)
4. Seeing the code work (hopefully...)

---

## What can I do?

We'd love your help in contributing to this repo. To pitch in:

1. Fork this repository
  - *Go in to this repo's main page, then click "Fork" on the top right*
2. Create a new branch
  - *This is where your new code will go*
  - *Do this by running:*
  ```bash  
   git checkout -b what-you-want-to-name-it
   ```
  - *Alternatively you can just click on the Branches dropdown and then create a new branch*
3. Commit the changes once you're done
  - *Do this by running:*
  ```bash  
   git commit -m "Write about what you did"  
   ```
4. Push, then pull!

---

## Help! I'm confused!
The [tutorials](https://lemlib.readthedocs.io/en/v0.5.0/tutorials/1_getting_started.html) provide a detailed walkthrough through all the features of LemLib. It covers everything from installation to path following.

---

## License
LemLib is licensed under the MIT license. Check [LICENSE](https://github.com/LemLib/LemLib/blob/master/LICENSE) for more details.

---

Special thanks to our friends from 1831E King's Team Echo for sharing their repo with us (this readme may or may not resemble theirs...)
