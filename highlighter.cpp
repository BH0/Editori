#include <QtGui>

#include "highlighter.h"

TextBlockData::TextBlockData() {
    // nothing to do
}

QVector<ParenthesisInfo *> TextBlockData::parentheses() {
    return m_parentheses;
}


void TextBlockData::insert(ParenthesisInfo *info) {
    int i = 0;
    while (i < m_parentheses.size() && info->position > m_parentheses.at(i)->position) {
        i++;
    }
    m_parentheses.insert(i, info);
}


Highlighter::Highlighter(QTextDocument *document) : QSyntaxHighlighter(document) {
    HighlightingRule rule;

       keywordFormat.setForeground(Qt::darkBlue);
       keywordFormat.setFontWeight(QFont::Bold);
       QStringList keywordPatterns; // it may help to extract the following patterns into their own file
       keywordPatterns // C/C++
                       << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                       << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                       << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                       << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                       << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                       << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                       << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                       << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                       << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                       << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"

                        // JavaScript
                       << "\\blet\\b" << "\\bconst\\b" << "\\bfunction\\b";

       foreach (const QString &pattern, keywordPatterns) {
           rule.pattern = QRegularExpression(pattern);
           rule.format = keywordFormat;
           highlightingRules.append(rule);
       }

       classFormat.setFontWeight(QFont::Bold);
       classFormat.setForeground(Qt::yellow);
       rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
       rule.format = classFormat;
       highlightingRules.append(rule);

       quotationFormat.setForeground(Qt::darkGreen);
       rule.pattern = QRegularExpression("\".*\"");
       rule.format = quotationFormat;
       highlightingRules.append(rule);

       functionFormat.setFontItalic(true);
       functionFormat.setForeground(Qt::darkRed);
       rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
       rule.format = functionFormat;
       highlightingRules.append(rule);

       singleLineCommentFormat.setForeground(Qt::green);
       rule.pattern = QRegularExpression("//[^\n]*");
       rule.format = singleLineCommentFormat;
       highlightingRules.append(rule);

       multiLineCommentFormat.setForeground(Qt::green);

       commentStartExpression = QRegularExpression("/\\*");
       commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text) {
    TextBlockData *data = new TextBlockData;

    int leftPos = text.indexOf('(');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '(';
        info->position = leftPos;

        data->insert(info);
        leftPos = text.indexOf('(', leftPos + 1);
    }

    int rightPos = text.indexOf(')');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = ')';
        info->position = rightPos;

        data->insert(info);

        rightPos = text.indexOf(')', rightPos +1);
    }

    setCurrentBlockUserData(data);

    foreach (const HighlightingRule &rule, highlightingRules) {
          QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
          while (matchIterator.hasNext()) {
              QRegularExpressionMatch match = matchIterator.next();
              setFormat(match.capturedStart(), match.capturedLength(), rule.format);
          }
      }

      setCurrentBlockState(0);

      int startIndex = 0;
      if (previousBlockState() != 1) {
          startIndex = text.indexOf(commentStartExpression);
      }

      while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

