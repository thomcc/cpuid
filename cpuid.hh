#ifndef CPUID_CPUID_HH
#define CPUID_CPUID_HH
#include <string>

// more info: http://www.flounder.com/cpuid_explorer2.htm

namespace cpuid {

namespace detail {
  static const unsigned HighBit = 0x80000000u;
  static const unsigned LowBit  = 0x00000001u;

  static const unsigned NoBits  = 0x00000000u;
  static const unsigned AllBits = 0xffffffffu;

  static inline unsigned NthBit(unsigned N) { return 1u << N; }

class CPUID {
  // BrandString makes assumptions about the layout of this struct.
  unsigned eax_, ebx_, ecx_, edx_;

public:

  CPUID(unsigned param) {
    reset(param);
  }

  void Reset(unsigned param) {
#if (defined __GNUC__)
    __asm__("cpuid;" : "=a"(eax_), "=b"(ebx_), "=c"(ecx_), "=d"(edx_) : "a"(param));
#else
# error "Don't know assembly syntax for your compiler"
#endif
  }

  unsigned eax() const { return eax_; }
  unsigned ebx() const { return ebx_; }
  unsigned ecx() const { return ecx_; }
  unsigned edx() const { return edx_; }
  static unsigned Eax(unsigned param) { return CPUID(param).eax_; }
  static unsigned Ebx(unsigned param) { return CPUID(param).ebx_; }
  static unsigned Ecx(unsigned param) { return CPUID(param).ecx_; }
  static unsigned Edx(unsigned param) { return CPUID(param).edx_; }

  static unsigned LoTests() { return CPUID(1u); }
  static unsigned HiTests() { return CPUID(0x80000001u); }

  static std::string BrandString(std::string const &if_unsupported="???") {
    if ((CPUID::Eax(0x80000000u) & 0x7fffffffu) < 4)
      return "";

    char str[1+48];
    char *p = str;

    for (unsigned arg = 0x80000002u; arg <= 0x80000004u; ++arg, p += sizeof(info)) {
      CPUID info(arg);
      memcpy(p, &info, sizeof info);
    }
    // TODO: is this already null terminated?
    str[48] = 0;
    return std::string(str);
  }

  static std::string VendorString() {
    char str[1+12]; // room for ebx_, ecx_, and edx_, null terminator
    char *p = str;
    CPUID info(0);
    memcpy(p, &info.ebx_, sizeof info.ebx_); p += sizeof info.ebx_;
    memcpy(p, &info.ecx_, sizeof info.ecx_); p += sizeof info.ecx_;
    memcpy(p, &info.edx_, sizeof info.edx_); p += sizeof info.edx_;
    *p = 0;
    return std::string(str);
  }

};

}


class CPUIDFlags {
public:
  CPUIDFlags() {
    detail::CPUID cpuid_hi(0x80000001u);
    detail::CPUID cpuid_lo(0x1u);

    hb_.value = cpuid_hi.ebx();
    hc_.value = cpuid_hi.ecx();
    hd_.value = cpuid_hi.edx();

    la_.value = cpuid_lo.eax();
    ld_.value = cpuid_hi.edx();

  }

  // common flags
  bool have_no_execute() const { return hc_.intel.no_execute; }
  bool have_sys_call()   const { return hc_.intel.sys_call_ret; }
  bool have_sys_ret()    const { return hc_.intel.sys_call_ret; }
  bool have_emt64t()     const { return hc_.intel.emt64t; }
  bool have_lahf()       const { return hd_.intel.lahf_sahf; }
  bool have_sahf()       const { return hd_.intel.lahf_sahf; }
  bool have_sse3()       const { return hd_.common.sse3; }
  bool have_monitor()    const { return hd_.common.monitor; }
  bool have_cpl()        const { return hd_.common.cpl; }
  bool have_vmx()        const { return hd_.common.vmx; }
  bool have_est()        const { return hd_.common.est; }
  bool have_tm2()        const { return hd_.common.tm2; }
  bool have_l1()         const { return hd_.common.l1; }
  bool have_cmpxchg16b() const { return hd_.common.cmpxchg16b; }
  bool have_xtpr()       const { return hd_.common.xtpr; }
  bool have_pdcm()       const { return hd_.common.pdcm; }
  bool have_dca()        const { return hd_.common.dca; }
  bool have_sse41()      const { return hd_.common.sse41; }
  bool have_sse42()      const { return hd_.common.sse42; }
  bool have_x2apic()     const { return hd_.common.x2apic; }
  bool have_mov_be()     const { return hd_.common.mov_be; }
  bool have_popcnt()     const { return hd_.common.popcnt; }
  bool have_aes()        const { return hd_.common.aes; }
  bool have_xsave()      const { return hd_.common.xsave; }
  bool have_osxsave()    const { return hd_.common.osxsave; }

  bool intel_have_fpu()   const { return ld_.intel.fpu; }
  bool intel_have_vme()   const { return ld_.intel.vme; }
  bool intel_have_de()    const { return ld_.intel.de; }
  bool intel_have_pse()   const { return ld_.intel.pse; }
  bool intel_have_msr()   const { return ld_.intel.msr; }
  bool intel_have_tsc()   const { return ld_.intel.tsc; }
  bool intel_have_pae()   const { return ld_.intel.pae; }
  bool intel_have_mce()   const { return ld_.intel.mce; }
  bool intel_have_cx8()   const { return ld_.intel.cx8; }
  bool intel_have_apic()  const { return ld_.intel.apic; }
  bool intel_have_pad_0() const { return ld_.intel.pad_0; }
  bool intel_have_sep()   const { return ld_.intel.sep; }
  bool intel_have_mtrr()  const { return ld_.intel.mtrr; }
  bool intel_have_pge()   const { return ld_.intel.pge; }
  bool intel_have_mca()   const { return ld_.intel.mca; }
  bool intel_have_cmov()  const { return ld_.intel.cmov; }
  bool intel_have_pat()   const { return ld_.intel.pat; }
  bool intel_have_pse36() const { return ld_.intel.pse36; }
  bool intel_have_psn()   const { return ld_.intel.psn; }
  bool intel_have_cflsh() const { return ld_.intel.cflsh; }
  bool intel_have_pad_1() const { return ld_.intel.pad_1; }
  bool intel_have_ds()    const { return ld_.intel.ds; }
  bool intel_have_acpi()  const { return ld_.intel.acpi; }
  bool intel_have_mmx()   const { return ld_.intel.mmx; }
  bool intel_have_fxsr()  const { return ld_.intel.fxsr; }
  bool intel_have_sse()   const { return ld_.intel.sse; }
  bool intel_have_sse2()  const { return ld_.intel.sse2; }
  bool intel_have_ss()    const { return ld_.intel.ss; }
  bool intel_have_htt()   const { return ld_.intel.htt; }
  bool intel_have_tm()    const { return ld_.intel.tm; }
  bool intel_have_pad_2() const { return ld_.intel.pad_2; }
  bool intel_have_pbe()   const { return ld_.intel.pbe; }


  // amd-specific flags
  bool amd_have_fpu()       const { return hc_.amd.fpu; }
  bool amd_have_vme()       const { return hc_.amd.vme; }
  bool amd_have_de()        const { return hc_.amd.de; }
  bool amd_have_pse()       const { return hc_.amd.pse; }
  bool amd_have_tsc()       const { return hc_.amd.tsc; }
  bool amd_have_msr()       const { return hc_.amd.msr; }
  bool amd_have_pae()       const { return hc_.amd.pae; }
  bool amd_have_mce()       const { return hc_.amd.mce; }
  bool amd_have_cmpxchg8b() const { return hc_.amd.cmpxchg8b; }
  bool amd_have_apic()      const { return hc_.amd.apic; }
  bool amd_have_mtrr()      const { return hc_.amd.mtrr; }
  bool amd_have_pge()       const { return hc_.amd.pge; }
  bool amd_have_mca()       const { return hc_.amd.mca; }
  bool amd_have_cmov()      const { return hc_.amd.cmov; }
  bool amd_have_pat()       const { return hc_.amd.pat; }
  bool amd_have_pse36()     const { return hc_.amd.pse36; }
  bool amd_have_mmx_ext()   const { return hc_.amd.mmx_ext; }
  bool amd_have_mmx()       const { return hc_.amd.mmx; }
  bool amd_have_fxsr()      const { return hc_.amd.fxsr; }
  bool amd_have_ffxsr()     const { return hc_.amd.ffxsr; }
  bool amd_have_rdtscp()    const { return hc_.amd.rdtscp; }
  bool amd_have_lm()        const { return hc_.amd.lm; }
  bool amd_have_3dnow_ext() const { return hc_.amd._3dnow_ext; }
  bool amd_have_3dnow()     const { return hc_.amd._3dnow; }

  bool amd_have_cmp_legacy()  const { return hd_.amd.cmp_legacy; }
  bool amd_have_svm()         const { return hd_.amd.svm; }
  bool amd_have_alt_mov_cr8() const { return hd_.amd.alt_mov_cr8; }

  unsigned intel_processor_type() const { return ha_.intel.proc_type; }

  unsigned raw_model_id() const { return ha_.amd.model_id; }
  unsigned raw_family_id() const { return ha_.amd.family_id; }
  unsigned raw_ext_model_id() const { return ha_.amd.ext_model_id; }
  unsigned raw_ext_family_id() const { return ha_.amd.ext_family_id; }
  unsigned stepping() const { return ha_.amd.step_id; }

  unsigned model() const {
    if (ha_.amd.family_id == 0x6 || ha_.amd.family_id == 0xf)
      return (ha_.amd.ext_model_id << 4) + ha_.amd.model_id;
    else
      return ha_.amd.model_id;
  }

  unsigned family() const {
    if (ha_.amd.family_id == 0xf)
      return (ha_.amd.ext_family_id << 4) + ha_.amd.family_id;
    else
      return ha_.amd.family_id;
  }

private:
  union {
    unsigned value;
    // intel is same as amd here, but intel has a processor type field.
    struct {
      unsigned step_id: 4;
      unsigned model_id: 4;
      unsigned family_id: 4;
      unsigned proc_type: 2;
      unsigned pad_0: 2;
      unsigned ext_model_id: 4;
      unsigned ext_family_id: 8;
      unsigned pad_1: 4;
    } intel;

    struct {
      unsigned step_id: 4;
      unsigned model_id: 4;
      unsigned family_id: 4;
      unsigned pad_0: 4;
      unsigned ext_model_id: 4;
      unsigned ext_family_id: 8;
      unsigned pad_1: 4;
    } amd;
  } la_;

  union {
    unsigned value;

    struct {
      unsigned fpu: 1;
      unsigned vme: 1;
      unsigned de: 1;
      unsigned pse: 1;
      unsigned msr: 1;
      unsigned tsc: 1;
      unsigned pae: 1;
      unsigned mce: 1;
      unsigned cx8: 1;
      unsigned apic: 1;
      unsigned pad_0: 1;
      unsigned sep: 1;
      unsigned mtrr: 1;
      unsigned pge: 1;
      unsigned mca: 1;
      unsigned cmov: 1;
      unsigned pat: 1;
      unsigned pse36: 1;
      unsigned psn: 1;
      unsigned cflsh: 1;
      unsigned pad_1: 1;
      unsigned ds: 1;
      unsigned acpi: 1;
      unsigned mmx: 1;
      unsigned fxsr: 1;
      unsigned sse: 1;
      unsigned sse2: 1;
      unsigned ss: 1;
      unsigned htt: 1;
      unsigned tm: 1;
      unsigned pad_2: 1;
      unsigned pbe: 1;
    } intel;

    struct {
      unsigned fpu: 1;
      unsigned vme: 1;
      unsigned de: 1;
      unsigned pse: 1;
      unsigned msr: 1;
      unsigned tsc: 1;
      unsigned pae: 1;
      unsigned mce: 1;
      unsigned cx8: 1;
      unsigned apic: 1;
      unsigned pad_0: 1;
      unsigned sep: 1;
      unsigned mtrr: 1;
      unsigned pge: 1;
      unsigned mca: 1;
      unsigned cmov: 1;
      unsigned pat: 1;
      unsigned pse36: 1;
      unsigned pad_1: 1;
      unsigned cflsh: 1;
      unsigned pad_2: 3;
      unsigned mmx: 1;
      unsigned fxsr: 1;
      unsigned sse: 1;
      unsigned sse2: 1;
      unsigned pad_3: 1;
      unsigned htt: 1;
      unsigned pad_4: 3;
    } amd;
  } ld_;

  union {
    unsigned value;
    // unused by intel
    struct {
      unsigned brand_id: 16;
      unsigned pad_: 16;
    } amd;
  } hb_;

  union {
    unsigned value;
    struct {
      unsigned pad_0: 11 // reserved
      unsigned sys_call_ret: 1;
      unsigned pad_1: 8; // reserved
      unsigned no_execute: 1;
      unsigned pad_2: 8; // reserved
      unsigned emt64t: 1;
      unsigned pad_3: 2; // reserved
    } intel;

    struct {
      unsigned fpu: 1;
      unsigned vme: 1;
      unsigned de: 1;
      unsigned pse: 1;
      unsigned tsc: 1;
      unsigned msr: 1;
      unsigned pae: 1;
      unsigned mce: 1;
      unsigned cmpxchg8b: 1;
      unsigned apic: 1;
      unsigned pad_0: 1; // reserved
      unsigned sys_call_ret: 1;
      unsigned mtrr: 1;
      unsigned pge: 1;
      unsigned mca: 1;
      unsigned cmov: 1;
      unsigned pat: 1;
      unsigned pse36: 1;
      unsigned pad_1: 2; // reserved
      unsigned no_execute: 1;
      unsigned pad_2: 1; // reserved
      unsigned mmx_ext: 1;
      unsigned mmx: 1;
      unsigned fxsr: 1;
      unsigned ffxsr: 1;
      unsigned pad_3: 1; // reserved
      unsigned rdtscp: 1;
      unsigned pad_4: 1; // reserved
      unsigned lm: 1;
      unsigned _3dnow_ext: 1;
      unsigned _3dnow: 1;
    } amd;

  } hc_;

  union {

    unsigned value;

    struct {
      unsigned lahf_sahf: 1;
      unsigned pad_: 31; // reserved
    } intel;

    struct {
      unsigned lahf_sahf: 1;
      unsigned cmp_legacy: 1;
      unsigned svm: 1;
      unsigned pad_0: 1; // reserved
      unsigned alt_mov_cr8: 1;
      unsigned pad_1: 26; // reserved
    } amd;

  } hd_;



  union {
    unsigned value;
    struct {
      unsigned sse3 : 1;
      unsigned pad_0 : 2;
      unsigned monitor : 1;
      unsigned cpl : 1;
      unsigned vmx : 1;
      unsigned pad_1 : 1;
      unsigned est : 1;
      unsigned tm2 : 1;

      unsigned pad_2 : 1;

      unsigned l1 : 1;

      unsigned pad_3 : 1;
      unsigned pad_4 : 1;

      unsigned cmpxchg16b : 1;

      unsigned xtpr: 1;
      unsigned pdcm: 1;

      unsigned pad_5 : 1;

      unsigned dca: 1;

      unsigned sse41: 1;
      unsigned sse42: 1;

      unsigned x2apic: 1;

      unsigned mov_be: 1;
      unsigned popcnt: 1;
      unsigned aes: 1;
      unsigned xsave: 1;
      unsigned osxsave: 1;

      unsigned pad_6 : 5;

    } common; // TODO: I know these aren't all common, which ones are on AMD?

  } lc_;



};

#if 0

class CacheData {
  CacheData() {}

private:
  // eax
  typedef union {
    unsigned value;

    struct {
      unsigned icache_count : 8;
      unsigned icache_assoc : 8;
      unsigned dcache_count : 8;
      unsigned dcache_assoc : 8;
    } amd;

  } tlb_2m4m;

  // ebx
  typedef union {
    unsigned value;

    struct {
      unsigned icount : 8;
      unsigned iassoc : 8;
      unsigned dcount : 8;
      unsigned dassoc : 8;
    } amd;

  } tlb_4k;

  // ecx
  typedef union {
     struct {
        unsigned line_size:8;
        unsigned tag_lines:8;
        unsigned assoc:8;
        unsigned size:8;
     } AMD;
     unsigned w;
  } l1_dcache;

  // edx
  typedef union {
     struct {
        unsigned line_size : 8;
        unsigned tag_lines : 8;
        unsigned assoc : 8;
        unsigned size : 8;
     } amd;
     unsigned w;
  } l1_icache;




};
#endif



}

}
#endif