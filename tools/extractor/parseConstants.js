const fs = require('fs')
const path = require('path')

// "Parses" flag values from provided header file
// I'd rather include this logic in extractor.cpp, but JS is so much better-
// suited to grab these values in this way. Let me know if you have a better idea.
const parseFile = async (filePath, startingDict = {}) => {
  let lines = await fs.promises.readFile(path.join(filePath), 'utf-8')
  lines = lines.split('\n')

  const output = {
    ...startingDict
  }

  lines.forEach((line) => {
    const match = line.match(/#define ([A-Z0-9x_]+)[ \t]+(?:(.*?)[ ]?)(?=(?:\/\/|$))/)
    if (match !== null) {
      let [_m, macroName, valueString, _comment] = match

      // Match things that look like variable names and replace them with already found values
      const symbols = valueString.matchAll(/(?<![A-Zx_0-9])(?<![0-9x])[A-Zx_][A-Z0-9x_]+/g)
      ;[...symbols].forEach(([symbol]) => {
        valueString = valueString.replace(symbol, output[symbol])
      })

      // At this point valueString should only hold arithmetic with number written in decimal or hex
      const result = eval(valueString)
      if (Number.isNaN(result) || result === undefined) {
        throw new Error(`Failed to eval string: "${line}"`)
      }

      output[macroName] = result
    }
  })

  for (const key in startingDict) {
    delete output[key]
  }

  return output
}

;(async () => {
  let output = {
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'map_groups.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'opponents.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'items.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'flags.h'), { MAX_TRAINERS_COUNT: 768 }),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'species.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'abilities.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'moves.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'songs.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'heal_locations.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'region_map_sections.h')),
    ...await parseFile(path.join(process.cwd(), 'include', 'constants', 'event_objects.h'))
  }

  ;[
    'FOCUS_PUNCH', 'DRAGON_CLAW', 'WATER_PULSE', 'CALM_MIND', 'ROAR', 'TOXIC', 'HAIL', 'BULK_UP', 'BULLET_SEED',
    'HIDDEN_POWER', 'SUNNY_DAY', 'TAUNT', 'ICE_BEAM', 'BLIZZARD', 'HYPER_BEAM', 'LIGHT_SCREEN', 'PROTECT', 'RAIN_DANCE',
    'GIGA_DRAIN', 'SAFEGUARD', 'FRUSTRATION', 'SOLAR_BEAM', 'IRON_TAIL', 'THUNDERBOLT', 'THUNDER', 'EARTHQUAKE',
    'RETURN', 'DIG', 'PSYCHIC', 'SHADOW_BALL', 'BRICK_BREAK', 'DOUBLE_TEAM', 'REFLECT', 'SHOCK_WAVE', 'FLAMETHROWER',
    'SLUDGE_BOMB', 'SANDSTORM', 'FIRE_BLAST', 'ROCK_TOMB', 'AERIAL_ACE', 'TORMENT', 'FACADE', 'SECRET_POWER', 'REST',
    'ATTRACT', 'THIEF', 'STEEL_WING', 'SKILL_SWAP', 'SNATCH', 'OVERHEAT'
  ].forEach((tmName, i) => {
    tmNumber = (i + 1).toString().padStart(2, '0')
    output[`ITEM_TM_${tmName}`] = output[`ITEM_TM${tmNumber}`]
  })

  ;['CUT', 'FLY', 'SURF', 'STRENGTH', 'FLASH', 'ROCK_SMASH', 'WATERFALL', 'DIVE'].forEach((hmName, i) => {
    hmNumber = (i + 1).toString().padStart(2, '0')
    output[`ITEM_HM_${hmName}`] = output[`ITEM_HM${hmNumber}`]
  })

  output = Object.entries(output).reduce((acc, [symbol, value]) => {
    if (symbol.startsWith('FLAG_UNUSED_')) {
      return acc
    }

    return {
      ...acc,
      [symbol]: value
    }
  }, {})

  await fs.promises.writeFile('./constants.json', JSON.stringify(output), 'utf-8')
})()
