# Circuit Impedance Solver

A command-line tool written in C for calculating equivalent impedance across
ten common passive component configurations. Built as a practical exploration
of circuit analysis in an embedded/firmware context.

---

## Supported Configurations

| # | Configuration      | Formula                               | Unit      |
|---|--------------------|---------------------------------------|-----------|
| 1 | Series Resistor    | R_eq = R1 + R2 + ... + Rn            | Ohms      |
| 2 | Series Capacitor   | 1/C_eq = 1/C1 + ... + 1/Cn           | Farads    |
| 3 | Series Inductor    | L_eq = L1 + L2 + ... + Ln            | Henries   |
| 4 | RC Circuit         | |Z| = sqrt(R^2 + Xc^2)               | Ohms |Z|  |
| 5 | RL Circuit         | |Z| = sqrt(R^2 + Xl^2)               | Ohms |Z|  |
| 6 | LC Circuit         | |Z| = |Xl - Xc|                      | Ohms |Z|  |
| 7 | RLC Circuit        | |Z| = sqrt(R^2 + (Xl - Xc)^2)        | Ohms |Z|  |
| 8 | Parallel Resistor  | 1/R_eq = 1/R1 + ... + 1/Rn           | Ohms      |
| 9 | Parallel Capacitor | C_eq = C1 + C2 + ... + Cn            | Farads    |
|10 | Parallel Inductor  | 1/L_eq = 1/L1 + ... + 1/Ln           | Henries   |

The combined circuits (RC, RL, LC, RLC) are frequency-dependent. The solver
prompts for operating frequency and returns impedance magnitude |Z|. LC and
RLC modes also display the resonant frequency f0 = 1 / (2*pi*sqrt(L*C)).

Supports up to 100 components per calculation.

---

## Getting Started

### Prerequisites

- GCC (or any C99-compatible compiler)
- Standard math library (`libm`, included on most Unix/Linux systems)

### Build

```bash
gcc -Wall -Wextra -std=c99 -o impedance main.c -lm
```

### Run

```bash
./impedance
```

### Example: Series RLC

```
============================================
        Circuit Impedance Solver
============================================
  Enter selection [1-11]: 7

-- Series RLC --
  Enter R value (Ohms): 100
  Enter L value (Henries): 0.01
  Enter C value (Farads): 0.000001
  Enter frequency (Hz): 1000

  (Resonant frequency: 1591.5494 Hz)

  Result: 148.745437 Ohms |Z|
```

---

## Project Structure

```
Circuit_Impedance_Solver/
├── main.c       # All source: helpers, solvers, menu loop
└── README.md
```

Single-file by design to keep the scope readable. A natural next step is
splitting into `solver.h` / `solver.c` as the feature set grows.

---

## Roadmap

- [ ] Phase angle output: theta = arctan(X / R)
- [ ] Parallel RC, RL, LC, RLC configurations
- [ ] Frequency sweep mode (print |Z| across a range)
- [ ] File-based batch input / CSV output
- [ ] Unit prefix parsing (e.g. `10k`, `4u7`, `100n`)

---

## Design Notes

**Why C?** Component solvers of this kind are common in embedded firmware
for filter design, impedance matching, and sensor interface validation.
Writing in C reinforces the precision habits (float vs. double, divide-by-zero
guards, `scanf` return checking) that matter on resource-constrained targets.

**Input validation.** Every solver guards against values that would produce
undefined arithmetic: zero capacitance in a series string, zero resistance in
a parallel bank, and non-positive frequencies for reactive calculations.

**`QUIT_CHOICE` macro.** The original code had a subtle bug: the menu loop
terminated on `choice != 4`, but Quit had been moved to option 11 as the menu
grew. Tying the sentinel to a named constant keeps the loop condition and the
printed menu in sync automatically.

**`ordinal()` helper.** Replaces the repeated if/else if chains (1st, 2nd,
3rd, nth) that appeared in every input function with a single two-line utility,
eliminating four identical blocks of duplicated logic.

---

## Author

Yoni Desta - April 2018
