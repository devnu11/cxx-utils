#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "include/pnext_range.hpp"

struct TestNode {
    int sType;
    const void* pNext;
    int value;
    
    TestNode(int type, const void* next, int val) 
        : sType(type), pNext(next), value(val) {}
};

struct CustomNode {
    int id;
    const void* nextPtr;
    std::string data;
    
    CustomNode(int i, const void* next, const std::string& d)
        : id(i), nextPtr(next), data(d) {}
};

template<>
struct PNextRangeTraits<CustomNode> {
    static inline const void* getNext(const CustomNode* node) {
        return node ? node->nextPtr : nullptr;
    }
};

class PNextRangeTraitsTest : public ::testing::Test {
protected:
    void SetUp() override {
        setupTestChain();
        setupCustomChain();
        setupSingleNodes();
    }
    
private:
    void setupTestChain() {
        node3 = std::make_unique<TestNode>(3, nullptr, 300);
        node2 = std::make_unique<TestNode>(2, node3.get(), 200);
        node1 = std::make_unique<TestNode>(1, node2.get(), 100);
    }
    
    void setupCustomChain() {
        customNode3 = std::make_unique<CustomNode>(103, nullptr, "third");
        customNode2 = std::make_unique<CustomNode>(102, customNode3.get(), "second");
        customNode1 = std::make_unique<CustomNode>(101, customNode2.get(), "first");
    }
    
    void setupSingleNodes() {
        singleNode = std::make_unique<TestNode>(42, nullptr, 999);
        singleCustomNode = std::make_unique<CustomNode>(201, nullptr, "single");
    }
    
protected:
    std::unique_ptr<TestNode> node1, node2, node3;
    std::unique_ptr<TestNode> singleNode;
    std::unique_ptr<CustomNode> customNode1, customNode2, customNode3;
    std::unique_ptr<CustomNode> singleCustomNode;
};

TEST_F(PNextRangeTraitsTest, DefaultTraitsConstruction) {
    PNextRange<const TestNode*> range(node1.get());
    
    ASSERT_FALSE(range.empty());
    EXPECT_EQ(range.front(), node1.get());
    EXPECT_EQ(range.size(), 3u);
}

TEST_F(PNextRangeTraitsTest, DefaultTraitsIteration) {
    PNextRange<const TestNode*> range(node1.get());
    
    std::vector<int> values = extractValues(range);
    
    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 100);
    EXPECT_EQ(values[1], 200);
    EXPECT_EQ(values[2], 300);
}

TEST_F(PNextRangeTraitsTest, CustomTraitsConstruction) {
    PNextRange<const CustomNode*> range(customNode1.get());
    
    ASSERT_FALSE(range.empty());
    EXPECT_EQ(range.front(), customNode1.get());
    EXPECT_EQ(range.size(), 3u);
}

TEST_F(PNextRangeTraitsTest, CustomTraitsIteration) {
    PNextRange<const CustomNode*> range(customNode1.get());
    
    std::vector<std::string> values = extractData(range);
    
    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], "first");
    EXPECT_EQ(values[1], "second");
    EXPECT_EQ(values[2], "third");
}

TEST_F(PNextRangeTraitsTest, CustomTraitsWithAlgorithms) {
    PNextRange<const CustomNode*> range(customNode1.get());
    
    auto it = findNodeWithData(range, "second");
    
    ASSERT_NE(it, range.end());
    EXPECT_EQ((*it)->id, 102);
}

TEST_F(PNextRangeTraitsTest, NullPointerConstruction) {
    PNextRange<const TestNode*> range1(nullptr);
    const void* nullVoid = nullptr;
    PNextRange<const TestNode*> range2(nullVoid);
    
    EXPECT_TRUE(range1.empty());
    EXPECT_TRUE(range2.empty());
    EXPECT_EQ(range1.size(), 0u);
    EXPECT_EQ(range2.size(), 0u);
}

TEST_F(PNextRangeTraitsTest, EmptyRangeIteration) {
    PNextRange<const TestNode*> range(nullptr);
    
    int count = countIterations(range);
    
    EXPECT_EQ(count, 0);
    EXPECT_EQ(range.begin(), range.end());
}

TEST_F(PNextRangeTraitsTest, SingleElementDefaultTraits) {
    PNextRange<const TestNode*> range(singleNode.get());
    
    EXPECT_FALSE(range.empty());
    EXPECT_EQ(range.size(), 1u);
    
    auto it = range.begin();
    EXPECT_NE(it, range.end());
    ++it;
    EXPECT_EQ(it, range.end());
}

TEST_F(PNextRangeTraitsTest, SingleElementCustomTraits) {
    PNextRange<const CustomNode*> range(singleCustomNode.get());
    
    EXPECT_FALSE(range.empty());
    EXPECT_EQ(range.size(), 1u);
    EXPECT_EQ(range.front()->data, "single");
}

TEST_F(PNextRangeTraitsTest, IteratorPreIncrement) {
    PNextRange<const TestNode*> range(node1.get());
    auto it = range.begin();
    
    EXPECT_EQ((*it)->value, 100);
    ++it;
    EXPECT_EQ((*it)->value, 200);
    ++it;
    EXPECT_EQ((*it)->value, 300);
    ++it;
    EXPECT_EQ(it, range.end());
}

TEST_F(PNextRangeTraitsTest, IteratorPostIncrement) {
    PNextRange<const TestNode*> range(node1.get());
    auto it = range.begin();
    
    auto prev = it++;
    EXPECT_EQ((*prev)->value, 100);
    EXPECT_EQ((*it)->value, 200);
}

TEST_F(PNextRangeTraitsTest, IteratorComparison) {
    PNextRange<const TestNode*> range(node1.get());
    
    verifyIteratorComparison(range);
}

TEST_F(PNextRangeTraitsTest, IteratorDereferencing) {
    PNextRange<const TestNode*> range(node1.get());
    auto it = range.begin();
    
    EXPECT_EQ((*it)->value, 100);
    EXPECT_EQ(it->value, 100);
}

TEST_F(PNextRangeTraitsTest, StdRangesCount) {
    PNextRange<const TestNode*> range(node1.get());
    
    auto count = std::ranges::count_if(range, [](const TestNode* node) {
        return node->value >= 200;
    });
    
    EXPECT_EQ(count, 2);
}

TEST_F(PNextRangeTraitsTest, StdRangesFind) {
    PNextRange<const TestNode*> range(node1.get());
    
    auto it = std::ranges::find_if(range, [](const TestNode* node) {
        return node->sType == 2;
    });
    
    ASSERT_NE(it, range.end());
    EXPECT_EQ((*it)->value, 200);
}

TEST_F(PNextRangeTraitsTest, StdRangesTransform) {
    PNextRange<const TestNode*> range(node1.get());
    
    std::vector<int> values;
    std::ranges::transform(range, std::back_inserter(values),
                          [](const TestNode* node) { return node->value; });
    
    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 100);
    EXPECT_EQ(values[1], 200);
    EXPECT_EQ(values[2], 300);
}

TEST_F(PNextRangeTraitsTest, StdRangesWithViews) {
    PNextRange<const TestNode*> range(node1.get());
    
    auto filtered = range | std::views::filter([](const TestNode* n) {
        return n->value > 150;
    }) | std::views::transform([](const TestNode* n) {
        return n->value * 2;
    });
    
    std::vector<int> results = collectResults(filtered);
    
    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[0], 400);
    EXPECT_EQ(results[1], 600);
}

TEST_F(PNextRangeTraitsTest, ConstRangeOperations) {
    const PNextRange<const TestNode*> range(node1.get());
    
    EXPECT_FALSE(range.empty());
    EXPECT_EQ(range.size(), 3u);
    EXPECT_EQ(range.front(), node1.get());
    
    verifyConstIteration(range);
}

TEST_F(PNextRangeTraitsTest, IncrementPastEnd) {
    PNextRange<const TestNode*> range(singleNode.get());
    
    auto it = range.begin();
    ++it;
    EXPECT_EQ(it, range.end());
    
    ++it;
    EXPECT_EQ(it, range.end());
}

TEST_F(PNextRangeTraitsTest, MultipleIndependentIterators) {
    PNextRange<const TestNode*> range(node1.get());
    
    verifyIndependentIterators(range);
}

TEST_F(PNextRangeTraitsTest, IteratorCopyAndAssignment) {
    PNextRange<const TestNode*> range(node1.get());
    
    verifyIteratorCopyAssignment(range);
}

TEST_F(PNextRangeTraitsTest, LongChain) {
    std::vector<std::unique_ptr<TestNode>> nodes = createLongChain(10000);
    
    PNextRange<const TestNode*> range(nodes.back().get());
    
    EXPECT_EQ(range.size(), 10000u);
    verifyLongChainValues(range);
}

TEST_F(PNextRangeTraitsTest, RangeConcepts) {
    verifyRangeConcepts();
}

TEST_F(PNextRangeTraitsTest, SimultaneousDefaultAndCustomTraits) {
    PNextRange<const TestNode*> defaultRange(node1.get());
    PNextRange<const CustomNode*> customRange(customNode1.get());
    
    EXPECT_EQ(defaultRange.size(), 3u);
    EXPECT_EQ(customRange.size(), 3u);
    
    EXPECT_EQ(defaultRange.front()->value, 100);
    EXPECT_EQ(customRange.front()->data, "first");
}

private:
    std::vector<int> extractValues(const PNextRange<const TestNode*>& range) {
        std::vector<int> values;
        for (const auto* node : range) {
            values.push_back(node->value);
        }
        return values;
    }
    
    std::vector<std::string> extractData(const PNextRange<const CustomNode*>& range) {
        std::vector<std::string> values;
        for (const auto* node : range) {
            values.push_back(node->data);
        }
        return values;
    }
    
    auto findNodeWithData(const PNextRange<const CustomNode*>& range, const std::string& target) {
        return std::ranges::find_if(range, [&target](const CustomNode* node) {
            return node->data == target;
        });
    }
    
    int countIterations(const PNextRange<const TestNode*>& range) {
        int count = 0;
        for ([[maybe_unused]] const auto* node : range) {
            ++count;
        }
        return count;
    }
    
    void verifyIteratorComparison(const PNextRange<const TestNode*>& range) {
        auto it1 = range.begin();
        auto it2 = range.begin();
        
        EXPECT_EQ(it1, it2);
        EXPECT_LE(it1, it2);
        EXPECT_GE(it1, it2);
        
        ++it1;
        EXPECT_NE(it1, it2);
        EXPECT_GT(it1, it2);
        EXPECT_LT(it2, it1);
    }
    
    template<typename Range>
    std::vector<int> collectResults(Range&& filtered) {
        std::vector<int> results;
        for (int val : filtered) {
            results.push_back(val);
        }
        return results;
    }
    
    void verifyConstIteration(const PNextRange<const TestNode*>& range) {
        for (const auto* node : range) {
            EXPECT_NE(node, nullptr);
        }
        
        auto it = range.cbegin();
        auto end = range.cend();
        EXPECT_NE(it, end);
    }
    
    void verifyIndependentIterators(const PNextRange<const TestNode*>& range) {
        auto it1 = range.begin();
        auto it2 = range.begin();
        
        ++it1;
        EXPECT_EQ((*it1)->value, 200);
        EXPECT_EQ((*it2)->value, 100);
        
        ++it2;
        ++it2;
        EXPECT_EQ((*it1)->value, 200);
        EXPECT_EQ((*it2)->value, 300);
    }
    
    void verifyIteratorCopyAssignment(const PNextRange<const TestNode*>& range) {
        auto it1 = range.begin();
        auto it2(it1);
        
        EXPECT_EQ(it1, it2);
        
        ++it1;
        auto it3 = range.begin();
        it3 = it1;
        
        EXPECT_EQ(it1, it3);
        EXPECT_NE(it2, it3);
    }
    
    std::vector<std::unique_ptr<TestNode>> createLongChain(int length) {
        std::vector<std::unique_ptr<TestNode>> nodes;
        
        for (int i = length - 1; i >= 0; --i) {
            const void* next = nodes.empty() ? nullptr : nodes.back().get();
            nodes.push_back(std::make_unique<TestNode>(i, next, i));
        }
        
        return nodes;
    }
    
    void verifyLongChainValues(const PNextRange<const TestNode*>& range) {
        int count = 0;
        for (const auto* node : range) {
            EXPECT_EQ(node->value, count);
            ++count;
        }
        EXPECT_EQ(count, 10000);
    }
    
    void verifyRangeConcepts() {
        using DefaultRange = PNextRange<const TestNode*>;
        using CustomRange = PNextRange<const CustomNode*>;
        
        static_assert(std::ranges::range<DefaultRange>);
        static_assert(std::ranges::forward_range<DefaultRange>);
        static_assert(std::ranges::input_range<DefaultRange>);
        
        static_assert(std::ranges::range<CustomRange>);
        static_assert(std::ranges::forward_range<CustomRange>);
        
        using DefaultIter = DefaultRange::Iterator;
        static_assert(std::forward_iterator<DefaultIter>);
        static_assert(std::input_iterator<DefaultIter>);
    }
};