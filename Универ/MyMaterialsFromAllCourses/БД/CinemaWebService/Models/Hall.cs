using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
 {
     public class Hall
     {
         [BsonId] // Указывает, что это поле является идентификатором документа
         [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
         public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
         public int HallId { get; set; } // PK
         public int Rating { get; set; }
         public int Capacity { get; set; }
         public string CleaningSchedule { get; set; }
         [BsonIgnore]
         public ICollection<Seat> Seats { get; set; } = new HashSet<Seat>();
         [BsonIgnore]
         public ICollection<Session> Sessions { get; set; } = new HashSet<Session>();
     }
 }