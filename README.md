<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>



<!-- PROJECT LOGO -->
<br />

<h1 align="center">C - Compilation project</h1>

[![C language logo][c-logo]](https://en.cppreference.com/w/c/language)

<!-- TABLE OF CONTENTS -->
<details>
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
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

The goal of this project is to design a compiler for a subset of the C language. The compiler will be written in C++ and will use ANTLR4. This project has been carried-out as an assignment for 4th years INSA's students (computer science).

[![INSA][insa-lgog]][insa-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

The number of dependencies is rather low however it's recommended to run the project using linux.

* [![Antlr][antlr-logo]][antlr-url]
* [![C++][cpp-logo]][cpp-url]
* [![Python][python-logo]][python-url]


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Before running a local copy of this project follow these steps.

### Antlr4

[Antlr4][antlr-url] is a common parser which is used here to build the [Abstract Syntax Tree (AST)][ast-url].

Linux installation:
```
mkdir ~/antlr4-install
```
```
cd ~/antlr4-install
```
If you want a specific version of Antlr, feel free to modify the following commands accordingly.
```
wget https://www.antlr.org/download/antlr-4.7.2-complete.jar
```
```
wget https://www.antlr.org/download/antlr4-cpp-runtime-4.7.2-source.zip
 ~/antlr4-install
```
```
unzip antlr4-cpp-runtime-4.7.2-source.zip
```
```
mkdir build
```
```
cd build
```
```
cmake -DCMAKE_INSTALL_PREFIX:PATH=~/antlr4-install/ ..
```
```
make -j8
```
```
make  install
```
Now edit the [config.mk][config-file]. It should look like this:
```
ANTLRJAR=/home/$(USER)/antlr4-install/antlr-4.7.2-complete.jar
ANTLRINC=/home/$(USER)/antlr4-install/include/antlr4-runtime/
ANTLRLIB=/home/$(USER)/antlr4-install/lib/libantlr4-runtime.a
ANTLR=java -jar ~/antlr4-install/antlr-4.7.2-complete.jar
```
> [!NOTE]
> You may need to uninstall previous installations of Antlr debending on your linux ditribution.

### Python

The test framework is based on [ifcc-test.py][python-test-script] which shouldn't need specific libraries.


<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Contributors:

<a href="https://github.com/Eleonore12345/4IF-pld-comp/graphs/contributors">
  <div>
  <img src="https://avatars.githubusercontent.com/u/68855403?v=4" width="40"/>
  AO2708
  </div>
  TODOOOOOO
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See [LICENSE.txt][license] for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



[insa-url]: https://www.insa-lyon.fr/
[antlr-url]: https://www.antlr.org/download.html
[cpp-url]: https://en.cppreference.com/w/cpp/17
[python-url]: https://www.python.org/downloads/
[ast-url]: https://en.wikipedia.org/wiki/Abstract_syntax_tree
[c-logo]: documents/c_icon.png
[insa-lgog]: documents/insa_logo.jpg 
[antlr-logo]: documents/antlr_logo.png
[cpp-logo]: documents/cpp_logo.png
[python-logo]: documents/python_logo.png
[config-file]: compiler/co
[python-test-script]: tests/ifcc-test.py
[license]: LICENCE.txt

[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew

[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 