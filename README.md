# CPUID

C++ utility library (header-only) to get information about x86[-64] processors.

Requires a compiler which understands gcc-style inline asm. Doesn't use exceptions or RTTI, but is still exception safe (the only functions that allocate resources are the two functions which return `std::strings`).

## API


    namespace cpuid {

      class CPUIDFlags {
      public:
        CPUIDFlags();
        // tests
        bool have_syscall() const;
        bool have_sysret() const;
        bool have_nx() const;
        bool have_mmx() const;
        bool have_mmx_ext() const;
        bool have_ffxsr() const;

        bool have_page_1gb() const;
        bool have_rdtscp() const;
        bool have_lm() const;
        bool have_3dnow_ext() const;
        bool have_3dnow() const;

        ALIAS(syscall, sysret);

        ALIAS(nx, xd); ALIAS(nx, no_execute);


        // ld
        bool have_fpu() const;
        bool have_vme() const;
        bool have_de() const;
        bool have_pse() const;
        bool have_tsc() const;
        bool have_msr() const;
        bool have_pae() const;
        bool have_mce() const;
        bool have_cmpxchg8b() const;
        bool have_apic() const;
        bool have_sys_enter_exit() const;
        bool have_mtrr() const;
        bool have_pge() const;
        bool have_mca() const;
        bool have_cmov() const;
        bool have_pat() const;
        bool have_pse36() const;
        bool have_clfsh() const;
        bool have_mmx() const;
        bool have_fxsr() const;
        bool have_sse() const;
        bool have_sse2() const;
        bool have_htt() const;
        bool have_tm() const;

        // lc
        bool have_lahf_sahf() const;
        bool have_legacy_cmp() const;
        bool have_svm() const;
        bool have_exp_apic_space() const;
        bool have_altmovcr8() const;
        bool have_abm() const;
        bool have_sse4a() const;
        bool have_misaligned_sse() const;
        bool have_3dnow_prefetch() const;
        bool have_osvw() const;
        bool have_ibs() const;






## Known bugs

- Some flags are invalid for certain processors (e.g. intel or amd specific flags
  don't error)

## License

MIT License

    Copyright (c) 2013 Thom Chiovoloni

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

