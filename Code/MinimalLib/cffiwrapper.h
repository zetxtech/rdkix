//
//  Copyright (C) 2021 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#pragma once

#include <RDGeneral/export.h>
#ifdef RDKIX_RDKIXCFFI_BUILD
#define RDKIX_RDKIXCFFI_EXPORT RDKIX_EXPORT_API
#else
#define RDKIX_RDKIXCFFI_EXPORT RDKIX_IMPORT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// I/O
RDKIX_RDKIXCFFI_EXPORT char *get_mol(const char *input, size_t *mol_sz,
                                     const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_qmol(const char *input, size_t *mol_sz,
                                      const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_molblock(const char *pkl, size_t pkl_sz,
                                          const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_v3kmolblock(const char *pkl, size_t pkl_sz,
                                             const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_smiles(const char *pkl, size_t pkl_sz,
                                        const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_smarts(const char *pkl, size_t pkl_sz,
                                        const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_cxsmiles(const char *pkl, size_t pkl_sz,
                                          const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_json(const char *pkl, size_t pkl_sz,
                                      const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_inchi(const char *pkl, size_t pkl_sz,
                                       const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_inchi_for_molblock(const char *ctab,
                                                    const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_inchikey_for_inchi(const char *inchi);
RDKIX_RDKIXCFFI_EXPORT char *get_rxn(const char *input, size_t *mol_sz,
                                     const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char **get_mol_frags(const char *pkl, size_t pkl_sz,
                                            size_t **frags_pkl_sz_array,
                                            size_t *num_frags,
                                            const char *details_json,
                                            char **mappings_json);

// substructure
RDKIX_RDKIXCFFI_EXPORT char *get_substruct_match(const char *mol_pkl,
                                                 size_t mol_pkl_sz,
                                                 const char *query_pkl,
                                                 size_t query_pkl_sz,
                                                 const char *options_json);
RDKIX_RDKIXCFFI_EXPORT char *get_substruct_matches(const char *mol_pkl,
                                                   size_t mol_pkl_sz,
                                                   const char *query_pkl,
                                                   size_t query_pkl_sz,
                                                   const char *options_json);

// Drawing
RDKIX_RDKIXCFFI_EXPORT char *get_svg(const char *pkl, size_t pkl_sz,
                                     const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_rxn_svg(const char *pkl, size_t pkl_sz,
                                         const char *details_json);

// Calculators
RDKIX_RDKIXCFFI_EXPORT char *get_descriptors(const char *pkl, size_t pkl_sz);
RDKIX_RDKIXCFFI_EXPORT char *get_morgan_fp(const char *pkl, size_t pkl_sz,
                                           const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_morgan_fp_as_bytes(const char *pkl,
                                                    size_t pkl_sz,
                                                    size_t *nbytes,
                                                    const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_rdkix_fp(const char *pkl, size_t pkl_sz,
                                          const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_rdkix_fp_as_bytes(const char *pkl,
                                                   size_t pkl_sz,
                                                   size_t *nbytes,
                                                   const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_pattern_fp(const char *pkl, size_t pkl_sz,
                                            const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_pattern_fp_as_bytes(const char *pkl,
                                                     size_t pkl_sz,
                                                     size_t *nbytes,
                                                     const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_topological_torsion_fp(
    const char *pkl, size_t pkl_sz, const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_topological_torsion_fp_as_bytes(
    const char *pkl, size_t pkl_sz, size_t *nbytes, const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_atom_pair_fp(const char *pkl, size_t pkl_sz,
                                              const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_atom_pair_fp_as_bytes(
    const char *pkl, size_t pkl_sz, size_t *nbytes, const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_maccs_fp(const char *pkl, size_t pkl_sz);
RDKIX_RDKIXCFFI_EXPORT char *get_maccs_fp_as_bytes(const char *pkl,
                                                   size_t pkl_sz,
                                                   size_t *nbytes);

#ifdef RDK_BUILD_AVALON_SUPPORT
RDKIX_RDKIXCFFI_EXPORT char *get_avalon_fp(const char *pkl, size_t pkl_sz,
                                           const char *details_json);
RDKIX_RDKIXCFFI_EXPORT char *get_avalon_fp_as_bytes(const char *pkl,
                                                    size_t pkl_sz,
                                                    size_t *nbytes,
                                                    const char *details_json);
#endif

// modification
RDKIX_RDKIXCFFI_EXPORT short add_hs(char **pkl, size_t *pkl_sz);
RDKIX_RDKIXCFFI_EXPORT short remove_all_hs(char **pkl, size_t *pkl_sz);

// standardization
RDKIX_RDKIXCFFI_EXPORT short cleanup(char **pkl, size_t *pkl_sz,
                                     const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short normalize(char **pkl, size_t *pkl_sz,
                                       const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short neutralize(char **pkl, size_t *pkl_sz,
                                        const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short reionize(char **pkl, size_t *pkl_sz,
                                      const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short canonical_tautomer(char **pkl, size_t *pkl_sz,
                                                const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short charge_parent(char **pkl, size_t *pkl_sz,
                                           const char *details_json);
RDKIX_RDKIXCFFI_EXPORT short fragment_parent(char **pkl, size_t *pkl_sz,
                                             const char *details_json);

// coordinates
RDKIX_RDKIXCFFI_EXPORT void prefer_coordgen(short val);
RDKIX_RDKIXCFFI_EXPORT short has_coords(char *mol_pkl, size_t mol_pkl_sz);
RDKIX_RDKIXCFFI_EXPORT short set_2d_coords(char **pkl, size_t *pkl_sz);
RDKIX_RDKIXCFFI_EXPORT short set_3d_coords(char **pkl, size_t *pkl_sz,
                                           const char *params_json);
RDKIX_RDKIXCFFI_EXPORT short set_2d_coords_aligned(char **pkl, size_t *pkl_sz,
                                                   const char *template_pkl,
                                                   size_t template_sz,
                                                   const char *details_json,
                                                   char **match_json);

// housekeeping
RDKIX_RDKIXCFFI_EXPORT void free_ptr(char *ptr);

RDKIX_RDKIXCFFI_EXPORT char *version();
RDKIX_RDKIXCFFI_EXPORT void enable_logging();
RDKIX_RDKIXCFFI_EXPORT void disable_logging();

// chirality
RDKIX_RDKIXCFFI_EXPORT short use_legacy_stereo_perception(short value);
RDKIX_RDKIXCFFI_EXPORT short allow_non_tetrahedral_chirality(short value);

// logging
RDKIX_RDKIXCFFI_EXPORT void *set_log_tee(const char *log_name);
RDKIX_RDKIXCFFI_EXPORT void *set_log_capture(const char *log_name);
RDKIX_RDKIXCFFI_EXPORT short destroy_log_handle(void **log_handle);
RDKIX_RDKIXCFFI_EXPORT char *get_log_buffer(void *log_handle);
RDKIX_RDKIXCFFI_EXPORT short clear_log_buffer(void *log_handle);

#ifdef __cplusplus
}
#endif
