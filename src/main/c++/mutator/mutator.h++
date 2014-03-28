#ifndef TYRANT_MUTATOR_MUTATOR_HPP
    #define TYRANT_MUTATOR_MUTATOR_HPP

    #include <memory>
    #include "mutationTask.h++"
    #include "mutationResult.h++"

    namespace Tyrant {
        namespace Mutator {

            class Mutator {
                public:
                    typedef std::shared_ptr<Mutator> Ptr;

                public:
                    virtual ~Mutator();
                    virtual MutationResult mutate(MutationTask const & task) = 0;
                    virtual Core::StaticDeckTemplate::ConstPtr quickMutate(Core::DeckTemplate::ConstPtr deck) = 0;
                    virtual void abort() = 0;

                    virtual bool isValid(Core::StaticDeckTemplate const & deck) const = 0;
                    virtual bool canCompose(Core::StaticDeckTemplate const & deck) const = 0;

            };

        }
    }

#endif
