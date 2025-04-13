<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

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
        <li><a href="#antlr4">Antlr4</a></li>
        <li><a href="#python">Python</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#error-handling-and-warnings">Error handling and warnings</a></li>
      </ul>
    </li>
    <li><a href="#example">Example</a></li>
    <li><a href="#contributors">Contibutors</a></li>
    <li><a href="#license">License</a></li>

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

> [!NOTE]
> You may need to uninstall previous installations of Antlr debending on your linux ditribution.

#### Linux installation:
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
Now create a `config.mk` file at the project's root, it should look like this:
```
ANTLRJAR=/home/$(USER)/antlr4-install/antlr-4.7.2-complete.jar
ANTLRINC=/home/$(USER)/antlr4-install/include/antlr4-runtime/
ANTLRLIB=/home/$(USER)/antlr4-install/lib/libantlr4-runtime.a
ANTLR=java -jar ~/antlr4-install/antlr-4.7.2-complete.jar
```

### Python

The test framework is based on [ifcc-test.py][python-test-script] which shouldn't need specific libraries.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Our compiler `ifcc` can compile basic code written in C. The only variable type allowed is `int` (even if we can declare single characters as int values). It allows function definitions, implements [`int putchar(int ch)`][putchar-url] and [`int getchar(int ch)`][getchar-url] and can raise a few specific errors and warnings.

Compilation target can be chosen between x86 and ARM architectures.

To compile the C++ code and make `ifcc` work, it's only required to enter the `make` command in [the compiler directory][compiler-dir].

In order to launch the compiler for x86 assembly, you need to :
```
./ifcc name_file.c
```
or
```
./ifcc -target x86 name_file.c
```

In order to launch the compiler for ARM assembly, you need to :
```
./ifcc -target arm name_file.c
```
NB : in order to launch the compiler for ARM assembly you need to install :
```
sudo apt install gcc-aarch64-linux-gnu libc6-arm64-cross qemu-user
sudo apt install gcc-aarch64-linux-gnu
```

If any issue appears :
```
sudo ln -s /usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 /lib/ld-linux-aarch64.so.1
sudo ln -s /usr/aarch64-linux-gnu/lib/libc.so.6 /lib/libc.so.6
```

Then you can have a look to the [python test framework][python-test-script] to see usage of `ifcc` for C programs compilation.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Error handling and warnings

`ifcc` can detect some cases of error and raise detailed messages inspired of `gcc` outputs. Here you can find all cases with corresponding references to code snippets. 

- `syntax error during parsing` [all cases not suported by grammar][err-syntax] (can be detailed by Antlr errors)
- `Variable <var> not declared`[undeclared variable or called outside its scope][err-var-non-decla] (Error)
- `Variable <var> already declared` [variable declared before in current scope][err-double-decla] (Error)
- `too few arguments to function '<function>'` [less arguments in function call than in function definition][err-too-few-args] (Error)
- `too many arguments to function '<function>'` [more arguments in function call than in function definition][err-too-many-args] (Error)
- `void value not ignored as it ought to be` [assignment to a variable of the return value of a void function][err-void-assign] (Error)
- `Function <function> already defined` [trying to define the same function twice][err-func-def-twice] (Error)
- `undefined reference to <function>` [call of a function without definition][err-undef-func] (Error)
- `undefined reference to main` [mandatory main function is missing][err-no-main] (Error)
- `variable <var> not used` [variable declared but not used][warn-not-used-var] (Warning)
- `conflicting types for '<function>'; have ‘void()’` [function called for assignment but defined as returning void later][warn-not-used-var] (Warning)
- `return with a value, in function returning void : <function>` [return expression in a void function][warn-return-void-func] (Warning)
- `implicit declaration of function fonction` [function declared after call][warn-not-used-var] (Warning)
- `variable <var> used but not initialized` [variable declared and used as rvalue without having been declared][warn-not-decl-var] (Warning)
- `variable <var> declared but not initialized` [variable declared but is never a lvalue][warn-not-decl-var] (Warning)
- `division by zero` [division by constant zero][warn-div-zero] (Warning)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Example

Our compiler produce correct assembly code for the following C code samples.

```C
int alphabet(int n) {
    int a;
    a='A';
    while (a<'A'+n)
    {
        putchar(a);
        a=a+1;
    }
    return a;
}

int main() {
    int c;
    c = alphabet(15);
    return c;
}
```

```C
int fibo(int n)
{
    if ( n < 1 )
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    return fibo(n-1) + fibo(n-2);
}

int main()
{
    return 2*fibo(8);
}
```

```C
void print_int(int x) {
    if(x<0)
    {
        putchar('-');
        x = -x;
    }
    if(x/10 != 0)
        print_int(x/10);
        
    putchar(x%10 + '0');
}

int main() {
    print_int(-273);
    putchar(10);
    return 0;
}
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributors:
<div style="display: flex; flex-wrap: wrap; gap: 12px;">
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/103212284?v=4" width="40" />
    <span><a href="https://github.com/yiwanou" >yiwanou</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/68855403?v=4" width="40" />
    <span><a href="https://github.com/AO2708" >AO2708</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/129035607?v=4" width="40" />
    <span><a href="https://github.com/Manon3005" >Manon3005</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/147798555?v=4" width="40" />
    <span><a href="https://github.com/armynion" >armynion</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/77687565?v=4" width="40" />
    <span><a href="https://github.com/Eleonore12345" >Eleonore12345</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/128641822?v=4" width="40" />
    <span><a href="https://github.com/BugaThis" >BugaThis</a></span>
  </div>
  <div style="display: flex; align-items: center; gap: 8px;">
    <img src="https://avatars.githubusercontent.com/u/127881703?v=4" width="40" />
    <span><a href="https://github.com/Bathrock78" >Bathrock78</a></span>
  </div>
</div>




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
[putchar-url]: https://en.cppreference.com/w/c/io/putchar
[getchar-url]: https://en.cppreference.com/w/c/io/getchar
[c-logo]: documents/c_icon.png
[insa-lgog]: documents/insa_logo.jpg 
[antlr-logo]: documents/antlr_logo.png
[cpp-logo]: documents/cpp_logo.png
[python-logo]: documents/python_logo.png
[compiler-dir]: compiler/
[python-test-script]: tests/ifcc-test.py
[license]: LICENCE.txt

[err-syntax]: tests/testfiles/testAffectation/erreur_affectation_multiple.c
[err-var-non-decla]: tests/testfiles/testAffectation/erreur_affectation_variable_non_declaree.c
[err-double-decla]: tests/testfiles/testDeclaration/erreur_redeclaration.c
[err-too-few-args]: tests/testfiles/testDefFonctions/erreur_appel_fonction_mauvais_nombre_param.c
[err-too-many-args]: tests/testfiles/testPutchar/putchar_2_args.c
[err-void-assign]: tests/testfiles/testDefFonctions/erreur_fonction_declaree_void_affectation_int.c
[err-func-def-twice]: tests/testfiles/testDefFonctions/erreur_fonction_def_twice.c
[err-undef-func]: tests/testfiles/testDefFonctions/erreur_fonction_sans_param_appelée_non_definie.c
[err-no-main]: tests/testfiles/testDefFonctions/erreur_prog_sans_main.c
[warn-return-void-func]: tests/testfiles/testTypeVoidFonction/warning_retour_valeur_alors_que_void.c
[warn-not-used-var]: tests/testfiles/testDefFonctions/warning_fonction_declaree_void_affectation_int.c
[warn-not-decl-var]: tests/testfiles/testWarningVariable/warning_affectation_variable_not_initialized.c
[warn-div-zero]: tests/testfiles/testDivision/division_par_const_0.c
