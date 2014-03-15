#include "cardChangeMutator.h++"
#include <errorHandling/assert.h++>
#include <errorHandling/exceptions.h++>
#include <core/cards/cardLoader.h++>
// for dynamic_pointer_cast
#include <memory>
#include <list>
#include <set>
#include <iostream>
#include <string>
#include "derefCompareLT.h++"
#include "cardChangeMutatorGenerator.h++"
#include "iterator/uniqueIterator.h++"

namespace Tyrant {
    namespace Mutator {

        CardChangeMutator::CardChangeMutator()
        {
            assertX(this->allowedCommanders.find(0) == this->allowedCommanders.end());
            //std::clog << "CCM::CCM() commanders: " << this->allowedCommanders.size() << std::endl;
        }

        CardChangeMutator::~CardChangeMutator()
        {
        }

        MutationResult
        CardChangeMutator::mutate(MutationTask const & task)
        {
            assertX(this->allowedCommanders.find(0) == this->allowedCommanders.end());
            // We only mutate certain deck types
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(task.baseDeck)) {
                CardChangeMutatorGenerator::CDeckSet input;
                input.insert(baseDeck);
                Ptr thisPtr = shared_from_this();
                CardChangeMutatorGenerator::Ptr generator = std::make_shared<CardChangeMutatorGenerator>(thisPtr, task, input);
                CardChangeMutatorGenerator::Ptr end       = std::make_shared<CardChangeMutatorGenerator>(thisPtr, task, input, true);

                DeckIterator uniqueCurrent(generator, end);
                DeckIterator uniqueEnd(end, end);

                MutationResult result(uniqueCurrent, uniqueEnd);
                return result;
            } else {
                throw InvalidUserInputError("Not supported.");
            }
        }

        Core::StaticDeckTemplate::ConstPtr
        CardChangeMutator::quickMutate
            (Core::DeckTemplate::ConstPtr deck)
        {
            if (Core::StaticDeckTemplate::ConstPtr baseDeck = std::dynamic_pointer_cast<Core::StaticDeckTemplate const>(deck)) {
                double roll = static_cast<double>(rand()) / RAND_MAX;
                double const flipOrderProbability = 0.1;
                double const removeCardProbability = 0.2;
                double const addCardProbability= 0.2;
                double const swapAdjacentCardProbability=0.1;
                if (roll < flipOrderProbability) {
                    if (this->isOrdered(*baseDeck)) {
                        return this->asUnordered(*baseDeck);
                    } else {
                        return this->asOrdered(*baseDeck);
                    }
                } else if (roll < flipOrderProbability + removeCardProbability) {
                    unsigned int const size = baseDeck->getNumberOfNonCommanderCards();
                    if (size > 0) {
                        unsigned int index = static_cast<unsigned int>(rand()) % size;
                        return baseDeck->withoutCardAtIndex(index);
                    } else {
                        return baseDeck;
                    }
                } else if (roll < flipOrderProbability + removeCardProbability + addCardProbability) {
                    unsigned int const size = baseDeck->getNumberOfNonCommanderCards();
                    if (size < 10) {
                        unsigned int index = static_cast<unsigned int>(rand()) % (size+1);
                        unsigned int cardId = this->getRandomOwnedNonCommander();
                        return baseDeck->withCardAtIndex(cardId, index);
                    } else {
                        return baseDeck;
                    }
                } else if (roll < flipOrderProbability + removeCardProbability + addCardProbability + swapAdjacentCardProbability) {
                    unsigned int const size = baseDeck->getNumberOfNonCommanderCards();
                    if (size > 1) {
                        unsigned int index = static_cast<unsigned int>(rand());
                        return baseDeck->withSwappedCards(index, index+1);
                    } else {
                        return baseDeck;
                    }
                } else {
                    unsigned int const size = baseDeck->getNumberOfNonCommanderCards();
                    unsigned int index = static_cast<unsigned int>(rand()) % size;
                    if(index == 0) {
                        unsigned int cardId = this->getRandomOwnedCommander();
                        return baseDeck->withCommander(cardId);
                    } else {
                        index--;
                        unsigned int cardId = this->getRandomOwnedNonCommander();
                        return baseDeck->withReplacedCardAtIndex(cardId, index);
                    }
                }
            } else {
                throw InvalidUserInputError("Not supported.");
            }
        }


        void
        CardChangeMutator::abort()
        {
            this->aborted = true;
        }

    }
}

