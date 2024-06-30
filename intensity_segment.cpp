#include "intensity_segment.h"
#include <iostream>
#include <sstream>

void IntensitySegments::add(int from, int to, int amount) {
    if (amount == 0 || from == to || from > to) {
        return;
    }

    // 如果为空
    if (m_location_map.empty()) {
        empty_insert(from, to, amount);
        return;
    }

    // 先将 end 做插入
    insert_to(from, to, amount);

    auto map_it = m_location_map.lower_bound(from);
    auto cur = map_it->second;
    // 没有查找到相同的值，则需要将from 作为新节点在之前的位置做插入。
    if (map_it->first != from) {
        // 处在首位需要特殊处理
        if (map_it == m_location_map.begin()) {
            m_segments_list.push_front(std::pair<int, int>(from, amount));
            m_location_map[from] = m_segments_list.begin();
            cur = m_segments_list.begin();
            ++cur;
        } else {
            auto pre = map_it->second;
            --pre;
            cur = m_segments_list.insert(map_it->second, std::pair<int, int>(from, amount + pre->second));
            m_location_map[from] = cur;
            ++cur;
        }
    } else {
        // 需要判断是否需要前置合并
        if (map_it != m_location_map.begin()) {
            auto loc = map_it->second;
            --loc;
            if (map_it->second->second + amount == loc->second) {
                cur = m_segments_list.erase(map_it->second);
                m_location_map.erase(from);
            } 
        }
    }
    
    // 运行到此处 cur 必定指向了 >= from 的节点。
    while (cur != m_segments_list.end() && cur->first < to) {
        cur->second += amount;
        ++cur;
    }
    
    // 由于已经插入了 to 节点，所以 cur 必然指向 to 
    // 判断是否需要后置合并
    auto pre = cur;
    --pre;
    if (cur->second == pre->second) {
        m_segments_list.erase(cur);
        m_location_map.erase(to);
    }

    // 判断第一个节点是否已经为 0
    if (m_segments_list.front().second == 0) {
        m_location_map.erase(m_segments_list.front().first);
        m_segments_list.pop_front();
    }
}

void IntensitySegments::empty_insert(int from, int to, int amount) {
    m_segments_list.push_front(std::pair<int, int>(from, amount));
    m_location_map[from] = m_segments_list.begin();
    m_segments_list.push_back(std::pair<int, int>(to, 0));
    auto loc = m_segments_list.end();
    --loc;
    m_location_map[to] = loc;
}

void IntensitySegments::insert_to(int from, int to, int amount) {
    auto map_to_it = m_location_map.lower_bound(to);
    if (map_to_it == m_location_map.end()) {
        // 重新设置结束点
        m_segments_list.push_back(std::pair<int, int>(to, 0));
        auto loc = m_segments_list.end();
        --loc;
        m_location_map[to] = loc;
    } else if (map_to_it->first != to) {
        // 需要将end 节点进行设置
        auto pre_it = map_to_it; 
        int pre = 0;
        if (pre_it != m_location_map.begin()) {
            --pre_it;
            pre = pre_it->second->second;
        }
        auto loc = m_segments_list.insert(map_to_it->second, std::pair<int, int>(to, pre));
        m_location_map[to] = loc;
    }
}

void IntensitySegments::set(int from, int to, int amount) {
    if (from == to || from > to) {
        return;
    }

    // 如果为空或者处于末尾则直接完成插入的过程
    if (m_location_map.empty()) {
        if (amount == 0) {
            return;
        }
        empty_insert(from, to, amount);
        return;
    }

    if (amount == 0) {
        if (from <= m_segments_list.front().first && to >= m_segments_list.back().first) {
            m_location_map.clear();
            m_segments_list.clear();
            return;
        } else if (to <= m_segments_list.front().first || from >= m_segments_list.back().first) {
            return;
        }
    }

    // 先将 end 做插入
    insert_to(from, to, amount);

    auto map_it = m_location_map.lower_bound(from);
    auto cur = map_it->second;
    // 没有查找到相同的值，则需要将from 作为新节点在之前的位置做插入。
    if (map_it->first != from) {
        // 处在首位需要特殊处理
        if (map_it == m_location_map.begin()) {
            m_segments_list.push_front(std::pair<int, int>(from, amount));
            m_location_map[from] = m_segments_list.begin();
            cur = m_segments_list.begin();
            ++cur;
        } else {
            cur = m_segments_list.insert(map_it->second, std::pair<int, int>(from, amount));
            m_location_map[from] = cur;
            ++cur;
        }
    } else {
        map_it->second->second = amount;
        // 需要判断是否需要前置合并
        if (map_it != m_location_map.begin()) {
            auto loc = map_it->second;
            --loc;
            if (amount == loc->second) {
                cur = m_segments_list.erase(map_it->second);
                m_location_map.erase(from);
            } else {
                ++cur;
            }
        } else {
            ++cur;
        }
    }
    
    // 运行到此处 cur 必定指向了 >=from 的节点。都需要清理。
    while (cur != m_segments_list.end() && cur->first < to) {
        auto clean = cur;
        ++cur;
        m_segments_list.erase(clean);
        m_location_map.erase(clean->first);
    }
    
    // 判断是否需要后置合并
    auto pre = cur;
    --pre;
    if (cur->second == pre->second) {
        m_segments_list.erase(cur);
        m_location_map.erase(to);
    }

    // 判断第一个节点是否已经为 0
    if (m_segments_list.front().second == 0) {
        m_location_map.erase(m_segments_list.front().first);
        m_segments_list.pop_front();
    }

    return;
}


std::string IntensitySegments::toString() {
    if (m_segments_list.empty()) {
        std::cout << "[]" << std::endl;
        return "[]";
    }

    std::stringstream ss;
    ss << "[";
    for (const auto& pair : m_segments_list) {
        ss << "[" << pair.first << "," << pair.second << "],";
    }
    std::string output = ss.str();
    output[output.length() - 1] = ']';
    std::cout << output << std::endl; 
    return output;
}