[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Issues][issues-shield]][issues-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/cdwr/simpleSSH">
    <img src="https://www.dmuth.org/wp-content/uploads/2020/01/ssh.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Simple SSH</h3>
  
<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://github.com/cdwr/simpleSSH)

This is an C example for a simple SSH client/server. It's configured to run on the localhost, but can be configured to run over most networks. It includes the most basic SSH commands: cd, pwd, mkdir, rmdir, rm, cat, get, and put.

### Built With

The SSH server and client were built for linux usage, and compiled with gcc. All dependencies 

* [gcc](https://gcc.gnu.org/)


<!-- GETTING STARTED -->
## Getting Started

This example was built for linux, but can be run using any C interpreter.

### Prerequisites

If you choose to recompile, you'll need a C compiler. I used gcc.
* gcc
  ```sh
  sudo apt install build-essential
  ```

### Installation

1) Clone the repo
   ```sh
   git clone https://github.com/cdwr/simpleSSH/
   ```
2) Check prerequisites

<!-- USAGE EXAMPLES -->
## Usage

Use case 1 (recommended):
  1) *optional* Compile the client and server
  ```sh
  gcc -o client client.c
  gcc -o server server.c
  ```
  2) Run the server
  ```sh
  ./server
  ```
  3) Run the client
  ```sh
  ./client
  ```
  4) Use built in commands
  ```sh
  pwd
  mkdir (dir_name)
  rmdir (dir_name)
  rm (dir_name)
  cat (file_name)
  get (file_name)
  put(file_name)
  ```
  
<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


<!-- CONTACT -->
## Contact

Chris Wilson - [@Chris Wilson](https://www.linkedin.com/in/chris-wilson-55882816b/)

Project Link: [https://github.com/cdwr/simpleSSH](https://github.com/cdwr/simpleSSH)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/cdwr/simpleSSH.svg?style=for-the-badge
[contributors-url]: https://github.com/cdwr/simpleSSH/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/cdwr/simpleSSH.svg?style=for-the-badge
[forks-url]: https://github.com/cdwr/simpleSSH/network/members
[issues-shield]: https://img.shields.io/github/issues/cdwr/simpleSSH.svg?style=for-the-badge
[issues-url]: https://github.com/cdwr/simpleSSH/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/chris-wilson-55882816b/
[product-screenshot]: Capture.PNG
