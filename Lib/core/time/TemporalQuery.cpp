//
// Created by admin on 13/12/24.
//

#include <core/time/Temporal.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        TemporalQuery const& TemporalQuery::ZONE_ID = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(ZONE_ID); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Zone ID";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::CHRONOLOGY = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(CHRONOLOGY); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Chronology";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::PRECISION = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(PRECISION); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Precision";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::OFFSET = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(OFFSET); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Zone Offset";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::ZONE = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try {
                        Optional<> id = temporal.query(ZONE_ID);
                        return id.isPresent() ? id : temporal.query(OFFSET);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Zone";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::LOCAL_DATE = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(LOCAL_DATE); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Local Date";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        TemporalQuery const& TemporalQuery::LOCAL_TIME = []() -> TemporalQuery& {
            class Query final : public TemporalQuery {
            public:
                Optional<> queryFrom(Temporal const& temporal) const override {
                    try { return temporal.query(LOCAL_TIME); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    return "Local Time";
                }
            };

            try { return UNSAFE::newInstance<Query>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }();

        Optional<> TemporalQuery::queryFrom(Temporal const& temporal) const {
            return Optional<>();
        }
    } // time
} // core
