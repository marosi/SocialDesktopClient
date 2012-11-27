/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "log.h"

namespace sdc {

Log::Level Log::global_level_;
std::string Log::level_name_[9];
std::string Log::time_format_ = "%H:%M:%S";

void Log::SetGlobalLevel(Level level) {
  Log::level_name_[Log::ERROR]   = "ERROR  ";
  Log::level_name_[Log::WARNING] = "WARNING";
  Log::level_name_[Log::INFO]    = "INFO   ";
  Log::level_name_[Log::TRACE]   = "TRACE  ";
  Log::level_name_[Log::DEBUG]   = "DEBUG  ";
  Log::level_name_[Log::DEBUG1]  = "DEBUG1 ";
  Log::level_name_[Log::DEBUG2]  = "DEBUG2 ";
  Log::level_name_[Log::DEBUG3]  = "DEBUG3 ";
  Log::level_name_[Log::DEBUG4]  = "DEBUG4 ";
  global_level_ = level;
}

Log::Level Log::GetGlobalLevel() {
  return global_level_;
}

void Log::TimeFormat(const std::string &format) {
  time_format_ = format;
}

Log::Log() {}

Log::~Log() {
  os << std::endl;
  std::cout << os.str();
  //fprintf(stderr, "%s", os.str().c_str());
  //fflush(stderr);
}

std::ostringstream& Log::Get(Level level, std::string producer) {
  //set one-time use level
  current_level_ = level;
  //log
  os << "[" << current_time() << "]";
  os << " " << level_name_[level] << " ";
  os << "= " << producer << " > ";
  os << std::string(level < DEBUG ? 0 : level - DEBUG, '\t');
  return os;
}

std::string Log::current_time() {
  time_t now;
  struct tm* tmnow;
  char buff[80];
  time(&now);
  tmnow = localtime(&now);
  strftime(buff, 80, time_format_.c_str(), tmnow);
  return std::string(buff);
}

} // namespace sdc
