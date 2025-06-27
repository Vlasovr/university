// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
// using System.Collections.Generic;
// using System.Linq;
// using System.Threading.Tasks;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class SessionMoviesController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public SessionMoviesController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<SessionMovieView>>> GetAllSessionMovies()
//         {
//             // Подгружаем Session, Movie (если нужно)
//             var sessionMovies = await _context.Set<SessionMovie>()
//                 .Include(sm => sm.Movie)
//                 .ToListAsync();
//
//             // Проецируем в SessionMovieView
//             var result = sessionMovies.Select(sm => new SessionMovieView
//             {
//                 SessionId   = sm.SessionId,
//                 MovieId     = sm.MovieId,
//                 // Пример: Берём Id (или что-то ещё) как SessionInfo
//                 SessionInfo = sm.SessionId.ToString(),
//                 // Предполагаем, что у Movie есть Title
//                 MovieInfo   = sm.Movie?.Name ?? string.Empty
//             }).ToList();
//
//             return Ok(result);
//         }
//         
//         [HttpGet("{sessionId}/{movieId}")]
//         public async Task<ActionResult<SessionMovieView>> GetSessionMovie(int sessionId, int movieId)
//         {
//             var sessionMovie = await _context.Set<SessionMovie>()
//                 // .Include(sm => sm.Session)
//                 .Include(sm => sm.Movie)
//                 .FirstOrDefaultAsync(sm => sm.SessionId == sessionId && sm.MovieId == movieId);
//
//             if (sessionMovie == null)
//             {
//                 return NotFound();
//             }
//
//             var view = new SessionMovieView
//             {
//                 SessionId   = sessionMovie.SessionId,
//                 MovieId     = sessionMovie.MovieId,
//                 SessionInfo = sessionMovie.SessionId.ToString(),
//                 MovieInfo   = sessionMovie.Movie?.Name ?? string.Empty
//             };
//
//             return Ok(view);
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<SessionMovie>> CreateSessionMovie(SessionMovie sessionMovie)
//         {
//             // Проверяем, что (SessionId, MovieId) валидны
//             if (!_context.Sessions.Any(s => s.SessionId == sessionMovie.SessionId) ||
//                 !_context.Movies.Any(m => m.MovieId == sessionMovie.MovieId))
//             {
//                 return BadRequest("Session или Movie не существует");
//             }
//             
//             bool alreadyExists = _context.Set<SessionMovie>()
//                 .Any(sm => sm.SessionId == sessionMovie.SessionId && sm.MovieId == sessionMovie.MovieId);
//
//             if (alreadyExists)
//             {
//                 return Conflict("Такая связка SessionId/MovieId уже существует");
//             }
//
//             _context.Set<SessionMovie>().Add(sessionMovie);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(
//                 nameof(GetSessionMovie),
//                 new { sessionId = sessionMovie.SessionId, movieId = sessionMovie.MovieId },
//                 sessionMovie
//             );
//         }
//         
//         [HttpDelete("{sessionId}/{movieId}")]
//         public async Task<IActionResult> DeleteSessionMovie(int sessionId, int movieId)
//         {
//             var sessionMovie = await _context.Set<SessionMovie>()
//                 .FindAsync(sessionId, movieId);
//
//             if (sessionMovie == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Set<SessionMovie>().Remove(sessionMovie);
//             await _context.SaveChangesAsync();
//             return NoContent();
//         }
//     }
//
//     public class SessionMovieView
//     {
//         public int SessionId { get; set; }
//         public int MovieId { get; set; }
//
//         public string SessionInfo { get; set; } = string.Empty;
//         public string MovieInfo { get; set; } = string.Empty;
//     }
// }


using Microsoft.AspNetCore.Mvc;
using MongoDB.Driver;
using CinemaWebService.Models;

namespace CinemaWebService.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class SessionMoviesController : ControllerBase
    {
        private readonly IMongoCollection<SessionMovie> _sessionMoviesCollection;
        private readonly IMongoCollection<Movie> _moviesCollection;
        private readonly IMongoCollection<Session> _sessionsCollection;

        public SessionMoviesController(IMongoDatabase database)
        {
            _sessionMoviesCollection = database.GetCollection<SessionMovie>("sessionMovies");
            _moviesCollection        = database.GetCollection<Movie>("movies");
            _sessionsCollection      = database.GetCollection<Session>("sessions");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<SessionMovieView>>> GetAllSessionMovies()
        {
            var sessionMovies = await _sessionMoviesCollection.Find(_ => true).ToListAsync();
            var movies = await _moviesCollection.Find(_ => true).ToListAsync();

            var result = sessionMovies.Select(sm =>
            {
                var movie = movies.FirstOrDefault(m => m.MovieId == sm.MovieId);
                return new SessionMovieView
                {
                    SessionId   = sm.SessionId,
                    MovieId     = sm.MovieId,
                    SessionInfo = sm.SessionId.ToString(),
                    MovieInfo   = movie?.Name ?? string.Empty
                };
            }).ToList();

            return Ok(result);
        }
        
        [HttpGet("{sessionId}/{movieId}")]
        public async Task<ActionResult<SessionMovieView>> GetSessionMovie(int sessionId, int movieId)
        {
            var filter = Builders<SessionMovie>.Filter.Eq(x => x.SessionId, sessionId) &
                         Builders<SessionMovie>.Filter.Eq(x => x.MovieId, movieId);

            var sessionMovie = await _sessionMoviesCollection.Find(filter).FirstOrDefaultAsync();
            if (sessionMovie == null)
            {
                return NotFound();
            }

            // Подтянем Movie
            var movie = await _moviesCollection
                .Find(m => m.MovieId == sessionMovie.MovieId).FirstOrDefaultAsync();

            var view = new SessionMovieView
            {
                SessionId   = sessionMovie.SessionId,
                MovieId     = sessionMovie.MovieId,
                SessionInfo = sessionMovie.SessionId.ToString(),
                MovieInfo   = movie?.Name ?? string.Empty
            };

            return Ok(view);
        }
        
        [HttpPost]
        public async Task<ActionResult<SessionMovie>> CreateSessionMovie(SessionMovie sessionMovie)
        {
            // Проверяем, что (SessionId, MovieId) валидны
            bool sessionExists = await _sessionsCollection.Find(s => s.SessionId == sessionMovie.SessionId).AnyAsync();
            bool movieExists   = await _moviesCollection.Find(m => m.MovieId == sessionMovie.MovieId).AnyAsync();

            if (!sessionExists || !movieExists)
            {
                return BadRequest("Session или Movie не существует");
            }
            
            // Проверяем дубли
            var filter = Builders<SessionMovie>.Filter.Eq(x => x.SessionId, sessionMovie.SessionId) &
                         Builders<SessionMovie>.Filter.Eq(x => x.MovieId, sessionMovie.MovieId);

            bool alreadyExists = await _sessionMoviesCollection.Find(filter).AnyAsync();
            if (alreadyExists)
            {
                return Conflict("Такая связка SessionId/MovieId уже существует");
            }

            await _sessionMoviesCollection.InsertOneAsync(sessionMovie);

            return CreatedAtAction(
                nameof(GetSessionMovie),
                new { sessionId = sessionMovie.SessionId, movieId = sessionMovie.MovieId },
                sessionMovie
            );
        }
        
        [HttpDelete("{sessionId}/{movieId}")]
        public async Task<IActionResult> DeleteSessionMovie(int sessionId, int movieId)
        {
            var filter = Builders<SessionMovie>.Filter.Eq(x => x.SessionId, sessionId) &
                         Builders<SessionMovie>.Filter.Eq(x => x.MovieId, movieId);

            var deleteResult = await _sessionMoviesCollection.DeleteOneAsync(filter);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }

    public class SessionMovieView
    {
        public int SessionId { get; set; }
        public int MovieId { get; set; }

        public string SessionInfo { get; set; } = string.Empty;
        public string MovieInfo   { get; set; } = string.Empty;
    }
}

