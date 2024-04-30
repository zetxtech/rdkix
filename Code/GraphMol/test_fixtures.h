//
//  Copyright (C) 2023 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#pragma once

#include <cstdlib>
#include <string>

#include <boost/noncopyable.hpp>

#include <GraphMol/Chirality.h>

class TestFixtureTemplate : public boost::noncopyable {
 public:
  TestFixtureTemplate() = delete;

  TestFixtureTemplate(std::string var, bool (*getter_func)(),
                      void (*setter_func)(bool))
      : m_getter_func{getter_func},
        m_setter_func{setter_func},
        m_var{std::move(var)} {
    auto evar = std::getenv(m_var.c_str());
    m_env_var_set = evar == nullptr;
    m_flag_state = (*m_getter_func)();
  }

  ~TestFixtureTemplate() {
    if (m_env_var_set) {
      (*m_setter_func)(m_flag_state);
    } else {
#ifdef _WIN32
      _putenv_s(m_var.c_str(), "");
#else
      unsetenv(m_var.c_str());
#endif
    }
  }

 protected:
  bool (*m_getter_func)();
  void (*m_setter_func)(bool);

 private:
  std::string m_var;

  bool m_flag_state;
  bool m_env_var_set;
};

class UseLegacyStereoPerceptionFixture : private TestFixtureTemplate {
 public:
  UseLegacyStereoPerceptionFixture()
      : TestFixtureTemplate(RDKix::Chirality::useLegacyStereoEnvVar,
                            &RDKix::Chirality::getUseLegacyStereoPerception,
                            &RDKix::Chirality::setUseLegacyStereoPerception) {}

  UseLegacyStereoPerceptionFixture(bool state)
      : TestFixtureTemplate(RDKix::Chirality::useLegacyStereoEnvVar,
                            &RDKix::Chirality::getUseLegacyStereoPerception,
                            &RDKix::Chirality::setUseLegacyStereoPerception) {
    (*m_setter_func)(state);
  }
};

class AllowNontetrahedralChiralityFixture : private TestFixtureTemplate {
 public:
  AllowNontetrahedralChiralityFixture()
      : TestFixtureTemplate(
            RDKix::Chirality::nonTetrahedralStereoEnvVar,
            &RDKix::Chirality::getAllowNontetrahedralChirality,
            &RDKix::Chirality::setAllowNontetrahedralChirality) {}
};