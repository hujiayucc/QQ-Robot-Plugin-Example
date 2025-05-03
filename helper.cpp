//
// Created by hujiayucc on 25-5-2.
//

#include <string>
#include <vector>
#include <random>
#include <sstream>

namespace Helper {
    const std::string IMAGE_MD_TEMPLATE = R"({
  "template_id": "模板ID",
  "components": {
    "img_header": {
      "props": {
        "style": {"width": "图片大小","height": "图片大小"},
        "src": "图片地址"
      }
    },
    "txt_content": {"props": {"text": "内容"}},
    "txt_footer": {"props": {"text": "一言"}}
  }
})";

    const std::string LINK_LIST_TEMPLATE = R"({
  "appid": "官方23Ark",
  "view": "[外显]",
  "items": [内容]
})";

    const std::string LINK_ITEM_TEMPLATE = R"({
  "type": "text",
  "text": {
    "content": "内容"
  }
})";

    const std::string THUMBNAIL_TEMPLATE = R"({
  "appid": "官方24Ark",
  "view": "[外显]",
  "items": [内容]
})";

    const std::string BIG_IMAGE_TEMPLATE = R"({
  "appid": "官方大图模板",
  "view": "顺风顺水顺人意",
  "image": {
    "url": "[URL]"
  }
})";

    /**
     * @brief 生成带随机箴言的图片MD模板
     * @param imageUrl 图片URL地址
     * @param templateId 模板ID
     * @param content 显示内容
     * @return 格式化后的JSON字符串
     */
    std::string GenerateImageMD(const std::string &imageUrl,
                                const std::string &templateId,
                                std::string content) {
        // 随机箴言库
        static const std::vector<std::string> WISDOMS = {
            "说一句谎话，要编造十句谎话来弥补，何苦呢？",
            "或许只需一滴露水，便能守护这绽放的花朵。",
            "这个月圣诞，下个月元旦，下下月完蛋。",
            "真正重要的东西，永远都是非常简单的。"
        };

        // 生成随机索引
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, WISDOMS.size() - 1);
        const std::string &wisdom = WISDOMS[dis(gen)];

        // 处理空内容
        if (content.empty()) content = " ";

        // 替换模板参数
        std::string result = IMAGE_MD_TEMPLATE;
        result.replace(result.find("模板ID"), 7, templateId);
        result.replace(result.find("图片地址"), 8, imageUrl);
        result.replace(result.find("内容"), 4, content);
        result.replace(result.find("一言"), 4, wisdom);

        // 图片尺寸处理（需根据实际需求实现）
        // 示例值：500x300
        if (const size_t sizePos = result.find("图片大小"); sizePos != std::string::npos) {
            result.replace(sizePos, 8, "\"500px\"");
        }

        return result;
    }

    /**
     * @brief 生成官方23Ark格式消息
     * @param content 内容文本（换行分隔）
     * @param displayText 外显文本
     * @return 格式化JSON
     */
    std::string GenerateOfficial23Ark(const std::string &content,
                                      const std::string &displayText = "") {
        std::vector<std::string> lines;
        std::istringstream iss(content);
        std::string line;

        // 分割文本行
        while (std::getline(iss, line, '\n')) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        // 构建条目
        std::string items;
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string item = LINK_ITEM_TEMPLATE;
            if (const size_t pos = item.find("内容"); pos != std::string::npos) {
                item.replace(pos, 4, lines[i]);
            }
            items += (i > 0 ? "," : "") + item;
        }

        // 组装完整模板
        std::string result = LINK_LIST_TEMPLATE;
        result.replace(result.find("[外显]"), 6,
                       displayText.empty() ? "顺风顺水顺人意" : displayText);
        result.replace(result.find("[内容]"), 6, items);

        return result;
    }

    /**
     * @brief 生成官方24Ark缩略图消息
     * @param content 内容文本
     * @param displayText 外显文本
     * @return 格式化JSON
     */
    std::string GenerateOfficial24Ark(const std::string &content,
                                      const std::string &displayText = "") {
        std::istringstream iss(content);
        std::string line;
        std::string items;

        // 构建条目
        int count = 0;
        while (std::getline(iss, line, '\n')) {
            if (!line.empty()) {
                std::string item = LINK_ITEM_TEMPLATE; // 使用相同条目模板
                if (const size_t pos = item.find("内容"); pos != std::string::npos) {
                    item.replace(pos, 4, line);
                }
                items += (count++ > 0 ? "," : "") + item;
            }
        }

        // 组装模板
        std::string result = THUMBNAIL_TEMPLATE;
        result.replace(result.find("[外显]"), 6,
                       displayText.empty() ? "顺风顺水顺人意" : displayText);
        result.replace(result.find("[内容]"), 6, items);

        return result;
    }

    /**
     * @brief 生成官方大图消息
     * @param imageUrl 图片URL
     * @return 格式化JSON
     */
    std::string GenerateBigImage(const std::string &imageUrl) {
        std::string result = BIG_IMAGE_TEMPLATE;
        if (const size_t pos = result.find("[URL]"); pos != std::string::npos) {
            result.replace(pos, 5, imageUrl);
        }
        return result;
    }
}
