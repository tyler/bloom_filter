require 'rake'
require 'spec/rake/spectask'

begin
  require 'jeweler'
  Jeweler::Tasks.new do |s|
    s.name = "bloom_filter"
    s.summary = 'Fast Bloom filter in C and Ruby.'
    s.email = "tyler@scribd.com"
    s.homepage = "http://github.com/tyler/bloom_filter"
    s.description = s.summary
    s.authors = ["Tyler McMullen"]
    s.extensions = ['ext/bloom_filter/extconf.rb']
    s.require_paths = ['ext','lib']
    s.files = FileList["[A-Z]*.*", "{lib,spec,ext}/**/*"]
    s.has_rdoc = false
  end
rescue LoadError
  puts "Jeweler not available. Install it with: sudo gem install technicalpickles-jeweler -s http://gems.github.com"
end

Spec::Rake::SpecTask.new do |t|
  t.spec_files = 'spec/**/*_spec.rb'
end

task :default => :spec
