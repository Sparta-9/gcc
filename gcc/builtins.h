/* Expand builtin functions.
   Copyright (C) 1988-2020 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_BUILTINS_H
#define GCC_BUILTINS_H

#include <mpc.h>

/* Target-dependent globals.  */
struct target_builtins {
  /* For each register that may be used for calling a function, this
     gives a mode used to copy the register's value.  VOIDmode indicates
     the register is not used for calling a function.  If the machine
     has register windows, this gives only the outbound registers.
     INCOMING_REGNO gives the corresponding inbound register.  */
  fixed_size_mode_pod x_apply_args_mode[FIRST_PSEUDO_REGISTER];

  /* For each register that may be used for returning values, this gives
     a mode used to copy the register's value.  VOIDmode indicates the
     register is not used for returning values.  If the machine has
     register windows, this gives only the outbound registers.
     INCOMING_REGNO gives the corresponding inbound register.  */
  fixed_size_mode_pod x_apply_result_mode[FIRST_PSEUDO_REGISTER];
};

extern struct target_builtins default_target_builtins;
#if SWITCHABLE_TARGET
extern struct target_builtins *this_target_builtins;
#else
#define this_target_builtins (&default_target_builtins)
#endif

/* Non-zero if __builtin_constant_p should be folded right away.  */
extern bool force_folding_builtin_constant_p;

extern bool called_as_built_in (tree);
extern bool get_object_alignment_1 (tree, unsigned int *,
				    unsigned HOST_WIDE_INT *);
extern unsigned int get_object_alignment (tree);
extern bool get_pointer_alignment_1 (tree, unsigned int *,
				     unsigned HOST_WIDE_INT *);
extern unsigned int get_pointer_alignment (tree);
extern unsigned string_length (const void*, unsigned, unsigned);

struct c_strlen_data
{
  /* [MINLEN, MAXBOUND, MAXLEN] is a range describing the length of
     one or more strings of possibly unknown length.  For a single
     string of known length the range is a constant where
     MINLEN == MAXBOUND == MAXLEN holds.
     For other strings, MINLEN is the length of the shortest known
     string.  MAXBOUND is the length of a string that could be stored
     in the largest array referenced by the expression.  MAXLEN is
     the length of the longest sequence of non-zero bytes
     in an object referenced by the expression.  For such strings,
     MINLEN <= MAXBOUND <= MAXLEN holds.  For example, given:
       struct A { char a[7], b[]; };
       extern struct A *p;
       n = strlen (p->a);
     the computed range will be [0, 6, ALL_ONES].
     However, for a conditional expression involving a string
     of known length and an array of unknown bound such as
       n = strlen (i ? p->b : "123");
     the range will be [3, 3, ALL_ONES].
     MINLEN != 0 && MAXLEN == ALL_ONES indicates that MINLEN is
     the length of the shortest known string and implies that
     the shortest possible string referenced by the expression may
     actually be the empty string.  This distinction is useful for
     diagnostics.  get_range_strlen() return value distinguishes
     between these two cases.
     As the tighter (and more optimistic) bound, MAXBOUND is suitable
     for diagnostics but not for optimization.
     As the more conservative bound, MAXLEN is intended to be used
     for optimization.  */
  tree minlen;
  tree maxlen;
  tree maxbound;
  /* When non-null, DECL refers to the declaration known to store
     an unterminated constant character array, as in:
     const char s[] = { 'a', 'b', 'c' };
     It is used to diagnose uses of such arrays in functions such as
     strlen() that expect a nul-terminated string as an argument.  */
  tree decl;
  /* Non-constant offset from the beginning of a string not accounted
     for in the length range.  Used to improve diagnostics.  */
  tree off;
};

extern tree c_strlen (tree, int, c_strlen_data * = NULL, unsigned = 1);
extern rtx c_readstr (const char *, scalar_int_mode, bool = true);
extern void expand_builtin_setjmp_setup (rtx, rtx);
extern void expand_builtin_setjmp_receiver (rtx);
extern void expand_builtin_update_setjmp_buf (rtx);
extern tree mathfn_built_in (tree, enum built_in_function fn);
extern tree mathfn_built_in (tree, combined_fn);
extern rtx builtin_strncpy_read_str (void *, HOST_WIDE_INT, scalar_int_mode);
extern rtx builtin_memset_read_str (void *, HOST_WIDE_INT, scalar_int_mode);
extern rtx expand_builtin_saveregs (void);
extern tree std_build_builtin_va_list (void);
extern tree std_fn_abi_va_list (tree);
extern tree std_canonical_va_list_type (tree);
extern void std_expand_builtin_va_start (tree, rtx);
extern void expand_builtin_trap (void);
extern void expand_ifn_atomic_bit_test_and (gcall *);
extern void expand_ifn_atomic_compare_exchange (gcall *);
extern rtx expand_builtin (tree, rtx, rtx, machine_mode, int);
extern enum built_in_function builtin_mathfn_code (const_tree);
extern tree fold_builtin_expect (location_t, tree, tree, tree, tree);
extern bool avoid_folding_inline_builtin (tree);
extern tree fold_call_expr (location_t, tree, bool);
extern tree fold_builtin_call_array (location_t, tree, tree, int, tree *);
extern bool validate_gimple_arglist (const gcall *, ...);
extern rtx default_expand_builtin (tree, rtx, rtx, machine_mode, int);
extern bool fold_builtin_next_arg (tree, bool);
extern tree do_mpc_arg2 (tree, tree, tree, int, int (*)(mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
extern tree fold_call_stmt (gcall *, bool);
extern void set_builtin_user_assembler_name (tree decl, const char *asmspec);
extern bool is_simple_builtin (tree);
extern bool is_inexpensive_builtin (tree);
extern bool readonly_data_expr (tree exp);
extern bool init_target_chars (void);
extern unsigned HOST_WIDE_INT target_newline;
extern unsigned HOST_WIDE_INT target_percent;
extern char target_percent_s[3];
extern char target_percent_c[3];
extern char target_percent_s_newline[4];
extern bool target_char_cst_p (tree t, char *p);

extern internal_fn associated_internal_fn (tree);
extern internal_fn replacement_internal_fn (gcall *);

extern bool check_nul_terminated_array (tree, tree, tree = NULL_TREE);
extern void warn_string_no_nul (location_t, tree, const char *, tree,
				tree, tree = NULL_TREE, bool = false,
				const wide_int[2] = NULL);
extern tree unterminated_array (tree, tree * = NULL, bool * = NULL);
extern bool builtin_with_linkage_p (tree);

/* Describes a reference to an object used in an access.  */
struct access_ref
{
  /* Set the bounds of the reference to at most as many bytes
     as the first argument or unknown when null, and at least
     one when the second argument is true unless the first one
     is a constant zero.  */
  access_ref (tree = NULL_TREE, bool = false);

  /* Reference to the accessed object(s).  */
  tree ref;

  /* Range of byte offsets into and sizes of the object(s).  */
  offset_int offrng[2];
  offset_int sizrng[2];
  /* Range of the bound of the access: denotes that the access
     is at least BNDRNG[0] bytes but no more than BNDRNG[1].
     For string functions the size of the actual access is
     further constrained by the length of the string.  */
  offset_int bndrng[2];
};

/* Describes a pair of references used in an access by built-in
   functions like memcpy.  */
struct access_data
{
  /* Set the access to at most MAXWRITE and MAXREAD bytes, and
     at least 1 when MINWRITE or MINREAD, respectively, is set.  */
  access_data (tree expr, access_mode mode,
	       tree maxwrite = NULL_TREE, bool minwrite = false,
	       tree maxread = NULL_TREE, bool minread = false)
    : call (expr),
      dst (maxwrite, minwrite), src (maxread, minread), mode (mode) { }

  /* Built-in function call.  */
  tree call;
  /* Destination and source of the access.  */
  access_ref dst, src;
  /* Read-only for functions like memcmp or strlen, write-only
     for memset, read-write for memcpy or strcat.  */
  access_mode mode;
};

class vr_values;
extern tree gimple_call_alloc_size (gimple *, wide_int[2] = NULL,
				    const vr_values * = NULL);
extern tree gimple_parm_array_size (tree, wide_int[2], const vr_values * = NULL);
extern tree compute_objsize (tree, int, tree * = NULL, tree * = NULL,
			     const vr_values * = NULL);
extern tree compute_objsize (tree, int, access_ref *, const vr_values * = NULL);

extern bool check_access (tree, tree, tree, tree, tree, access_mode,
			  const access_data * = NULL);

#endif /* GCC_BUILTINS_H */
