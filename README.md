# parallel-yet-more-primes

<a id="readme-top"></a>

<div align="center">
  <h1 align="center">Yet More Primes — Sequential vs. Parallel (OpenMP)</h1>
</div>

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
        <li><a href="#installation">Installation</a></li>
        <li><a href="#usage">Usage</a></li>
      </ul>
    </li>
    <li><a href="#contributors">Contributors</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

This project solves **Problem E — Yet More Primes**, from the WSCAD Marathon of Parallel Programming. A scientist took `P` prime numbers, cut each one into two halves at an arbitrary point, and shuffled all `2P` halves together. The program must test **every possible combination** of (first half `i`) + (second half `j`), check which concatenations are prime, and print the primes found in ascending order.

The repository contains two implementations:

- **[`sequential/`](sequential/)** — the reference baseline, unmodified trial-division algorithm (`isprime()` divides by every factor up to `√value`).
- **[`parallel/`](parallel/)** — the same algorithm parallelized with **OpenMP**. The `(i,j)` double loop (where 100% of the computational work lives) is distributed across threads with `#pragma omp for collapse(2) schedule(dynamic, CHUNK)`. Since each thread finds an unknown number of primes, results are collected in a private buffer per thread and merged into the shared array once at the end of the parallel region — avoiding both race conditions and unnecessary `critical`/`atomic` overhead per result.

Test inputs of varying size (`input_10_primes`, `input_20_primes`, `input_50_primes`, `input_100_primes`, ...) and their expected outputs (`tests/expected_outputs/`) are included to validate correctness and measure speedup between the two versions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

- ![C][C-url]
- ![OpenMP][OpenMP-url]
- ![GNU Make][Make-url]
- ![Bash][Bash-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

### Installation

1. Clone the repository
2. Make sure you have a C compiler with OpenMP support (e.g. GCC) available as `CC`

### Usage

**Sequential baseline:**
```bash
cd sequential
make
./primetest < input_20_primes
```

**Parallel (OpenMP) version:**
```bash
cd parallel
make
./primetest-parallel < input_20_primes
```

The number of threads used by the parallel version is fixed at compile time via `NUM_THREADS` in [`parallel/primetest.c`](parallel/primetest.c) — change it and run `make` again to test a different thread count.

**Validating correctness:**
```bash
diff <(./parallel/primetest-parallel < parallel/input_20_primes) tests/expected_outputs/output_20_primes
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributors

<a href="https://github.com/Jeferson-Filho/parallel-yet-more-primes/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Jeferson-Filho/parallel-yet-more-primes" alt="contrib.rocks image" />
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Caio Bohlhalter <br>
[![LinkedIn][linkedin-shield]][caio-linkedin-url]

Jeferson Filho <br>
[![LinkedIn][linkedin-shield]][jeferson-linkedin-url]

Murilo Venturato <br>
[![LinkedIn][linkedin-shield]][murilo-linkedin-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[C-url]: https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white
[OpenMP-url]: https://img.shields.io/badge/OpenMP-orange?style=for-the-badge
[Make-url]: https://img.shields.io/badge/GNU%20Make-427819?style=for-the-badge&logo=gnu&logoColor=white
[Bash-url]: https://img.shields.io/badge/GNU%20Bash-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white
[Python-url]: https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54
[GitHub-url]: https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white
<!-- -------------------------------------------------------------------------------- -->
[caio-linkedin-url]: https://www.linkedin.com/in/caio-bohlhalter-de-souza-202646232/
[jeferson-linkedin-url]: https://www.linkedin.com/in/jdietrichfho/
[murilo-linkedin-url]: https://www.linkedin.com/in/murilo-venturato-7450a1207/
