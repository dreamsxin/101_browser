theory stdlib
imports Main
begin

definition MinUint32 :: int where "MinUint32 \<equiv> 0"
definition MaxUint32 :: int where "MaxUint32 \<equiv> (2^32) + (-1) "

typedef Uint32 = "{i::int. MinUint32 \<le> i \<and> i \<le> MaxUint32}"
by(unfold MinUint32_def MaxUint32_def, auto)

definition MinUint64 :: int where "MinUint64 \<equiv> 0"
definition MaxUint64 :: int where "MaxUint64 \<equiv> (2^64) + (-1) "

typedef Uint64 = "{i::int. MinUint64 \<le> i \<and> i \<le> MaxUint64}"
by(unfold MinUint64_def MaxUint64_def, auto)

typedef size_t32 = Uint32
by(unfold Uint32_def MinUint32_def MaxUint32_def, auto)


end
