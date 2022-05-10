---
layout: page
title: Week 1 - Principles & Practices, Program Management
permalink: w1-principles.html
---

## Assignment 1: [Principles & Practices][Principles]

### Potential Final Project

> Plan and sketch a potential final project

I had shortlisted a few ideas like a micro-drone and a rolling robot (like Sphero). However, our instructor's ([Francisco][Francisco]) 
advice was to make something personal, "something that wouldn't be made if you didn't make it". This echoed Prof. Gershenfeld's 
[insight][MarketOf1Quote] that "the killer app for personal fabrication is the ability to make products for a market of 1".

So, with that in mind I'd like to create...

#### Hi! Time

<img src="images/clock.jpg" height="300"/>

Hi! Time is a digital clock that recognizes gestures like hand-waves.

It is meant especially for the night when you wake up in the dark and have to know the time, but you don't want a clock
 that annoyingly tick-tocks in the silence or one which has a display that is bright in the dark, which prevents you from 
 [falling into a deep sleep][DeepSleep].

So it is a clock, which:

* has an LED / LCD display
* detects ambient light and displays the time normally in the day
* but turns off the display when its completely dark
* it detects hand motion, maybe a hand-wave and turns the display on for a few seconds
* (optional) projects the time so that the image lands on the opposite wall and is visible for near-sighted people like me
* (optional) speaks out the time if it hears a finger snap

Note: this gesture recognition only happens when the clock is in "dark" mode, not in the day when there is a lot of 
movement and noise. In the day, it works like a normal LED clock.

&nbsp;

---

&nbsp;

## Assingment 2: [Project Management][ProjectManagement]

> Build a personal site in the class archive describing you and your final project

### Building this site

This site is built using [Jekyll][Jekyll] and [Markdown][Markdown].

I liked Markdown's simple syntax and needed a way to construct a static website where I did not need to repeat the 
common site structure like headers and footers. Jekyll provides a pretty nice way to do this.

Installing Jekyll involved installing Ruby 2.2, which I did through RVM (Ruby Version Manager), and then installing 
Jekyll through rubygems.
 
Jekyll is a parsing engine, it uses [Liquid][Liquid] markup for template and site layout definition, and generates a 
static html site when you run `$ jekyll build`.

It also has a convenient built-in webserver that automatically rebuilds and serves your static site when you make 
changes. Very convenient during development. It can be launched with `$ jekyll serve`.

For Fab Academy projects, which require pages to be loadable as static html, its important to *only use relative urls*
only. Also do not use pretty urls for permalinks like "/about" (which require a webserver to load /about/index.html), 
instead use "about.html" (relative url, directly pointing to the page).

With the above approach, you need to build the site each time you change something and want to view it in the browser. 
Here's a script to regenrate the static site whenever you save a file in your source directory. You need to 
`apt-get install inotify-tools` for this:

`$ while inotifywait -e modify .; do jekyll build; done`

The jekyll source for my site is checked in to the Trivandrum repo. You can clone it: 
  git@git.fabacademy.org:fabacademy2016/fablabtrivandrum.git, and look at the source in students/yogi directory.
  
After making changes to markdown files in students/yogi, I run 
`$ jekyll build --destination ../424` from /students/yogi directory to generate static content into /students/424 directory.

Once this is done, I check in both the source in /students/yogi and the generated content in /students/424. 


### Uploading files to the  archive

I've been using git for many years now, so am quite familiar with it. However, I did review the documentation on [Git-Scm].

The setup steps involved were:

* Generate a separate key-pair using `$ ssh-keygen` (enter a strong passpahrase) which generates id_rsa & id_rsa.pub in ~/.ssh folder. 
* `chmod 600 ~/.ssh/id_rsa` to make it readable and writable only by you.
* Copy the public key to http://git.fabacademy.org/profile/keys.
* Test the setup by running `$ ssh git@git.fabacademy.org`. If everything works fine should print `Welcome to GitLab, yogi!`
* If it didn't work it could be because: 
    * ssh private key wasn't added to ssh-agent. For this run `$ ssh-add`, which will prompt for your passphrase once and add 
        id_rsa private key to ssh-agent. Once this is done you don't need to enter the passphrase each time.
    * You might not be able to git push if you are not marked as a Developer in GitLab, please ask Fransisco to add you to that role.

Typical workflow:

* Make changes to files in students/yogi.
* run `$ jekyll build --destination ../424` from /students/yogi directory to generate static content into /students/424 directory.
* `git add :/` to stage all files in git.
* `git commit -m 'some message'`.
* `git pull`, this will typically require a merge, but there won't be any conflicts since all students make changes in their
 own sub-directories. 
* `git push`

Occasionally I will create a local branch for my changes using `git checkout -b week2`, and keep committing to this branch. 
Eventually I would merge into master by doing `git checkout master` and `git merge week2`. 

Note: I prefer to do a git merge than rebase because rebase rewrites previous commits. I don't see the advantage of 
this - merge shows that files had diverged and needed merging, which reflects what actually happened.


### Describe yourself

Done in the [About Me][About] page.

[Jekyll]: http://jekyllrb.com
[Markdown]: https://daringfireball.net/projects/markdown/syntax
[Bootstrap]: http://getbootstrap.com/
[Francisco]: http://fabacademy.org/archives/2013/students/sanchez.francisco/
[Principles]: http://academy.cba.mit.edu/classes/principles_practices/index.html
[ProjectManagement]: http://academy.cba.mit.edu/classes/project_management/index.html
[Liquid]: http://liquidmarkup.org/
[About]: about.html
[MarketOf1Quote]: http://archive.fortune.com/magazines/fortune/fortune_archive/2006/11/13/8393124/index.htm
[DeepSleep]: http://io9.gizmodo.com/why-we-need-to-sleep-in-total-darkness-1497075228
[Git-Scm]: https://git-scm.com/