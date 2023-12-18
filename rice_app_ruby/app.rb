# frozen_string_literal: true

require 'time'

# Simulates a rice cooker with various functionalities
class RiceCooker
  attr_accessor :functionalities

  # Initializes RiceCooker class
  def initialize
    @functionalities = [
      'Cooking rice',
      'Keep warm',
      'Cooking schedule',
      'Quantity guide'
    ]
    @stop_maintain = nil
  end

  # Displays available functionalities
  def show_functionalities
    puts 'Rice Cooker Functionalities:'
    @functionalities.each_with_index do |functionality, index|
      puts "#{index + 1}. #{functionality}"
    end
  end

  # Allows user to choose a menu option
  def choose_option
    puts 'Choose the menu:'
    gets.chomp.to_i
  end

  # Stops maintaining warmness
  def stop_maintain_warm
    return unless @stop_maintain&.alive?

    @stop_maintain.kill
    @stop_maintain = nil
  end

  # Keeps rice warm for a specified duration
  def keep_warm(duration)
    puts 'The keep warm functionality is activated.'

    start_time = Time.now
    loop do
      elapsed_time = Time.now - start_time
      remaining_time = duration - elapsed_time

      minutes = (remaining_time / 60).to_i
      seconds = (remaining_time % 60).to_i

      printf("\rKeeping warm... %<minutes>02d:%<seconds>02d", minutes: minutes, seconds: seconds)

      break if remaining_time <= 0

      sleep(1)
    end

    puts "\nKeeping rice warm is completed."
  end

  # Guides about quantities required for a certain number of persons
  def guide_quantities(persons)
    loop do
      puts "For #{persons} person(s), you will need:"
      quantity_rice, quantity_water = calculate_quantities(persons)
      puts "- Quantity of rice: #{format('%.2f', quantity_rice)} g"
      puts "- Quantity of water: #{format('%.2f', quantity_water)} ml"

      puts 'Press 1 to start cooking, 2 to change the number of persons, or Enter to quit.'
      choice = gets.chomp.downcase

      case choice
      when '1'
        puts 'Starting rice cooking.'
        cook_rice
        return
      when '2'
        puts 'Changing the number of persons.'
        persons = choose_number_of_persons
      else
        puts 'Quit.'
        return
      end
    end
  end

  # Allows user to choose the number of persons
  def choose_number_of_persons
    puts 'Enter the number of persons:'
    gets.chomp.to_i
  end

  # Keeps rice warm during a countdown
  def keep_warm_during_countdown(duration)
    puts 'The keep warm functionality is activated.'

    loop do
      remaining_time = duration - Time.now

      minutes = (remaining_time / 60).to_i
      seconds = (remaining_time % 60).to_i

      printf("\rKeeping warm... %<minutes>02d:%<seconds>02d", minutes: minutes, seconds: seconds)

      break if remaining_time <= 0

      sleep(1)
    end

    puts "\nKeeping rice warm is completed."
  end

  # Cooks rice based on specified details
  def cook_rice
    details = obtain_cooking_details

    return unless valid_cooking_details?(details)

    display_cooking_details(details)
    start_cooking(details)
  end

  # Obtains cooking details from user input
  def obtain_cooking_details
    details = {
      water_liters: 0,
      timer: 0
    }

    puts 'Add the quantity of water:'
    details[:water_liters] = gets.chomp.to_f

    puts 'Set the cooking timer (in minutes):'
    details[:timer] = gets.chomp.to_i

    details
  end

  # Validates cooking details
  def valid_cooking_details?(details)
    if details[:water_liters] <= 0 || details[:timer] <= 0
      puts 'Cooking details are incomplete. Cooking cannot be started.'
      return false
    end
    true
  end

  # Displays details about starting rice cooking
  def display_cooking_details(details)
    puts "Starting rice cooking for #{format('%.2f', details[:water_liters])} ml of water for #{details[:timer]} minutes."
  end

  # Starts the cooking process
  def start_cooking(details)
    done = false
    @stop_maintain = Thread.new do
      details[:timer].times do |i|
        sleep(1)
        puts "Cooking... #{i + 1} minute(s) passed"
      end
      done = true
    end

    @stop_maintain.join if @stop_maintain
    puts 'Cooking is finished. Proceeding to keep warm functionality...'
    duration = Time.now + details[:timer] * 60
    keep_warm_during_countdown(duration)
  end

  # Calculates quantities of rice and water required for a number of persons
  def calculate_quantities(persons)
    quantity_rice_per_person = 100.0 # In grams
    quantity_water_per_person = 200.0 # In milliliters
    [persons * quantity_rice_per_person, persons * quantity_water_per_person]
  end
end

rice_cooker = RiceCooker.new

loop do
  rice_cooker.show_functionalities

  choice = rice_cooker.choose_option

  case choice
  when 1
    puts 'Starting rice cooking.'
    rice_cooker.cook_rice
    rice_cooker.stop_maintain_warm
  when 2
    puts 'Keeping the cooked rice warm.'
    rice_cooker.keep_rice_warm
  else
    puts 'Functionality not implemented.'
  end

  puts 'Press Enter to return to the main menu or enter \'q\' to quit.'
  quit = gets.chomp
  break if quit.downcase == 'q'
end

puts 'Press Enter to quit.'
gets.chomp



# linter : https://www.honeybadger.io/blog/linting-formatting-ruby/