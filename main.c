/**
 * @file    main.c
 * @brief   Circuit Impedance Solver — command-line tool for calculating
 *          equivalent impedance of passive component networks.
 *
 * Supported configurations:
 *   Series:   Resistor, Capacitor, Inductor
 *   Combined: RC, RL, LC, RLC  (frequency-dependent, returns |Z|)
 *   Parallel: Resistor, Capacitor, Inductor
 *
 * Build:  gcc -Wall -Wextra -std=c99 -o impedance main.c -lm
 * Run:    ./impedance
 *
 * Created by Yoni Desta, April 2018.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* -------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------- */
#define MAX_COMPONENTS  100
#define TWO_PI          (2.0f * 3.14159265358979f)

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

/**
 * @brief  English ordinal suffix for a 1-based position.
 *         1 -> "st", 2 -> "nd", 3 -> "rd", everything else -> "th".
 */
static const char *ordinal(int n) {
    if (n == 1) return "st";
    if (n == 2) return "nd";
    if (n == 3) return "rd";
    return "th";
}

/**
 * @brief  Read and validate a component count from stdin.
 *
 * @param  name  Component label used in the prompt (e.g. "Resistor").
 * @return Validated integer in [1, MAX_COMPONENTS].
 */
static int read_count(const char *name) {
    int count = 0;
    printf("  Enter %s count: ", name);
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_COMPONENTS) {
        fprintf(stderr, "  Error: count must be between 1 and %d.\n",
                MAX_COMPONENTS);
        exit(EXIT_FAILURE);
    }
    return count;
}

/**
 * @brief  Prompt the user to enter @p count component values into @p arr.
 *
 * @param  arr    Output array (caller must provide at least @p count elements).
 * @param  count  Number of values to read.
 * @param  label  Short symbol for the component, e.g. "R", "C", "L".
 * @param  unit   Unit string appended to each prompt, e.g. "Ohms".
 */
static void read_values(float arr[], int count,
                        const char *label, const char *unit) {
    for (int i = 0; i < count; i++) {
        printf("  Enter %d%s %s value (%s): ",
               i + 1, ordinal(i + 1), label, unit);
        if (scanf("%f", &arr[i]) != 1 || arr[i] < 0.0f) {
            fprintf(stderr, "  Error: value must be a non-negative number.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief  Read a single positive float (e.g. frequency) from stdin.
 *
 * @param  prompt  Full prompt string printed before reading.
 * @return The validated float value.
 */
static float read_positive_float(const char *prompt) {
    float val = 0.0f;
    printf("  %s", prompt);
    if (scanf("%f", &val) != 1 || val <= 0.0f) {
        fprintf(stderr, "  Error: value must be positive.\n");
        exit(EXIT_FAILURE);
    }
    return val;
}

/* -------------------------------------------------------------------------
 * Series solvers
 * ---------------------------------------------------------------------- */

/**
 * @brief  Series resistors: R_eq = R1 + R2 + ... + Rn
 * @return Equivalent resistance in Ohms.
 */
static float series_resistor(void) {
    float arr[MAX_COMPONENTS];
    int   count  = read_count("Resistor");
    float result = 0.0f;

    read_values(arr, count, "R", "Ohms");
    for (int i = 0; i < count; i++) result += arr[i];
    return result;
}

/**
 * @brief  Series capacitors: 1/C_eq = 1/C1 + 1/C2 + ... + 1/Cn
 * @return Equivalent capacitance in Farads.
 */
static float series_capacitor(void) {
    float arr[MAX_COMPONENTS];
    int   count      = read_count("Capacitor");
    float reciprocal = 0.0f;

    read_values(arr, count, "C", "Farads");
    for (int i = 0; i < count; i++) {
        if (arr[i] == 0.0f) {
            fprintf(stderr, "  Error: capacitance cannot be zero.\n");
            exit(EXIT_FAILURE);
        }
        reciprocal += 1.0f / arr[i];
    }
    return 1.0f / reciprocal;
}

/**
 * @brief  Series inductors: L_eq = L1 + L2 + ... + Ln
 *         (assumes negligible mutual inductance between coils)
 * @return Equivalent inductance in Henries.
 */
static float series_inductor(void) {
    float arr[MAX_COMPONENTS];
    int   count  = read_count("Inductor");
    float result = 0.0f;

    read_values(arr, count, "L", "Henries");
    for (int i = 0; i < count; i++) result += arr[i];
    return result;
}

/* -------------------------------------------------------------------------
 * Frequency-dependent combined solvers
 *
 * All return impedance magnitude |Z| at the supplied frequency.
 * ---------------------------------------------------------------------- */

/**
 * @brief  Series RC circuit:
 *           Xc = 1 / (2*pi*f*C)
 *           |Z| = sqrt(R^2 + Xc^2)
 *
 * @return Impedance magnitude in Ohms.
 */
static float resistor_capacitor(void) {
    float R = read_positive_float("Enter R value (Ohms): ");
    float C = read_positive_float("Enter C value (Farads): ");
    float f = read_positive_float("Enter frequency (Hz): ");

    float xc = 1.0f / (TWO_PI * f * C);
    return sqrtf(R * R + xc * xc);
}

/**
 * @brief  Series RL circuit:
 *           Xl = 2*pi*f*L
 *           |Z| = sqrt(R^2 + Xl^2)
 *
 * @return Impedance magnitude in Ohms.
 */
static float resistor_inductor(void) {
    float R = read_positive_float("Enter R value (Ohms): ");
    float L = read_positive_float("Enter L value (Henries): ");
    float f = read_positive_float("Enter frequency (Hz): ");

    float xl = TWO_PI * f * L;
    return sqrtf(R * R + xl * xl);
}

/**
 * @brief  Series LC circuit:
 *           Xl = 2*pi*f*L,   Xc = 1 / (2*pi*f*C)
 *           |Z| = |Xl - Xc|
 *
 *         At resonance (f0), Xl = Xc so |Z| = 0 — the circuit is a short.
 *         f0 = 1 / (2*pi*sqrt(L*C))
 *
 * @return Impedance magnitude in Ohms.
 */
static float inductor_capacitor(void) {
    float L = read_positive_float("Enter L value (Henries): ");
    float C = read_positive_float("Enter C value (Farads): ");
    float f = read_positive_float("Enter frequency (Hz): ");

    float xl = TWO_PI * f * L;
    float xc = 1.0f / (TWO_PI * f * C);
    float f0 = 1.0f / (TWO_PI * sqrtf(L * C));

    printf("  (Resonant frequency: %.4f Hz — |Z| = 0 at this frequency)\n", f0);
    return fabsf(xl - xc);
}

/**
 * @brief  Series RLC circuit:
 *           Xl = 2*pi*f*L,   Xc = 1 / (2*pi*f*C)
 *           |Z| = sqrt(R^2 + (Xl - Xc)^2)
 *         At resonance (f0), Xl = Xc so |Z| = R — the minimum possible impedance.
 *         Unlike a pure LC circuit, resistance prevents |Z| from reaching zero.
 *         f0 = 1 / (2*pi*sqrt(L*C))
 *
 * @return Impedance magnitude in Ohms.
 */
static float resistor_inductor_capacitor(void) {
    float R = read_positive_float("Enter R value (Ohms): ");
    float L = read_positive_float("Enter L value (Henries): ");
    float C = read_positive_float("Enter C value (Farads): ");
    float f = read_positive_float("Enter frequency (Hz): ");

    float xl  = TWO_PI * f * L;
    float xc  = 1.0f / (TWO_PI * f * C);
    float net = xl - xc;
    float f0  = 1.0f / (TWO_PI * sqrtf(L * C));

    printf("  (Resonant frequency: %.4f Hz — |Z| is minimum (= R) at this frequency)\n", f0);
    return sqrtf(R * R + net * net);
}

/* -------------------------------------------------------------------------
 * Parallel solvers
 * ---------------------------------------------------------------------- */

/**
 * @brief  Parallel resistors: 1/R_eq = 1/R1 + 1/R2 + ... + 1/Rn
 * @return Equivalent resistance in Ohms.
 */
static float parallel_resistor(void) {
    float arr[MAX_COMPONENTS];
    int   count      = read_count("Resistor");
    float reciprocal = 0.0f;

    read_values(arr, count, "R", "Ohms");
    for (int i = 0; i < count; i++) {
        if (arr[i] == 0.0f) {
            fprintf(stderr, "  Error: resistance cannot be zero (short circuit).\n");
            exit(EXIT_FAILURE);
        }
        reciprocal += 1.0f / arr[i];
    }
    return 1.0f / reciprocal;
}

/**
 * @brief  Parallel capacitors: C_eq = C1 + C2 + ... + Cn
 * @return Equivalent capacitance in Farads.
 */
static float parallel_capacitor(void) {
    float arr[MAX_COMPONENTS];
    int   count  = read_count("Capacitor");
    float result = 0.0f;

    read_values(arr, count, "C", "Farads");
    for (int i = 0; i < count; i++) result += arr[i];
    return result;
}

/**
 * @brief  Parallel inductors: 1/L_eq = 1/L1 + 1/L2 + ... + 1/Ln
 *         (assumes negligible mutual inductance between coils)
 * @return Equivalent inductance in Henries.
 */
static float parallel_inductor(void) {
    float arr[MAX_COMPONENTS];
    int   count      = read_count("Inductor");
    float reciprocal = 0.0f;

    read_values(arr, count, "L", "Henries");
    for (int i = 0; i < count; i++) {
        if (arr[i] == 0.0f) {
            fprintf(stderr, "  Error: inductance cannot be zero.\n");
            exit(EXIT_FAILURE);
        }
        reciprocal += 1.0f / arr[i];
    }
    return 1.0f / reciprocal;
}

/* -------------------------------------------------------------------------
 * Menu
 * ---------------------------------------------------------------------- */

#define QUIT_CHOICE 11

static void print_menu(void) {
    printf("\n============================================\n");
    printf("        Circuit Impedance Solver\n");
    printf("============================================\n");
    printf("  Select a circuit configuration:\n\n");
    printf("   --- Series ---\n");
    printf("  [ 1] Series Resistor\n");
    printf("  [ 2] Series Capacitor\n");
    printf("  [ 3] Series Inductor\n\n");
    printf("   --- Combined (frequency-dependent) ---\n");
    printf("  [ 4] RC  (Resistor-Capacitor)\n");
    printf("  [ 5] RL  (Resistor-Inductor)\n");
    printf("  [ 6] LC  (Inductor-Capacitor)\n");
    printf("  [ 7] RLC (Resistor-Inductor-Capacitor)\n\n");
    printf("   --- Parallel ---\n");
    printf("  [ 8] Parallel Resistor\n");
    printf("  [ 9] Parallel Capacitor\n");
    printf("  [10] Parallel Inductor\n\n");
    printf("  [%d] Quit\n", QUIT_CHOICE);
    printf("--------------------------------------------\n");
    printf("  Enter selection [1-%d]: ", QUIT_CHOICE);
}

static void main_menu(void) {
    int choice;

    do {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF); /* flush bad input */
            choice = 0;
            continue;
        }

        float       result = 0.0f;
        const char *unit   = "Ohms";

        switch (choice) {
            /* --- Series --- */
            case 1:
                printf("\n-- Series Resistor --\n");
                result = series_resistor();   unit = "Ohms";    break;
            case 2:
                printf("\n-- Series Capacitor --\n");
                result = series_capacitor();  unit = "Farads";  break;
            case 3:
                printf("\n-- Series Inductor --\n");
                result = series_inductor();   unit = "Henries"; break;

            /* --- Combined --- */
            case 4:
                printf("\n-- Series RC --\n");
                result = resistor_capacitor();          unit = "Ohms |Z|"; break;
            case 5:
                printf("\n-- Series RL --\n");
                result = resistor_inductor();           unit = "Ohms |Z|"; break;
            case 6:
                printf("\n-- Series LC --\n");
                result = inductor_capacitor();          unit = "Ohms |Z|"; break;
            case 7:
                printf("\n-- Series RLC --\n");
                result = resistor_inductor_capacitor(); unit = "Ohms |Z|"; break;

            /* --- Parallel --- */
            case 8:
                printf("\n-- Parallel Resistor --\n");
                result = parallel_resistor();   unit = "Ohms";    break;
            case 9:
                printf("\n-- Parallel Capacitor --\n");
                result = parallel_capacitor();  unit = "Farads";  break;
            case 10:
                printf("\n-- Parallel Inductor --\n");
                result = parallel_inductor();   unit = "Henries"; break;

            case QUIT_CHOICE:
                printf("\nGoodbye.\n\n"); break;

            default:
                printf("  Invalid selection. Please enter 1-%d.\n",
                       QUIT_CHOICE);
                break;
        }

        if (choice >= 1 && choice < QUIT_CHOICE) {
            printf("\n  Result: %.6f %s\n", result, unit);
        }

    } while (choice != QUIT_CHOICE);
}

/* -------------------------------------------------------------------------
 * Entry point
 * ---------------------------------------------------------------------- */

int main(void) {
    main_menu();
    return EXIT_SUCCESS;
}
